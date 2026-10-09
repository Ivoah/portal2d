#!/usr/bin/env -S scala-cli shebang

//> using scala 3.8.4
//> using jvm 21

import java.nio.file.{Path, Files}
import scala.jdk.StreamConverters.*
import scala.sys.process.*
import scala.math.Ordering.Implicits.infixOrderingOps

val EXE = "portal2d"

val SOURCE_DIR = Path.of("src")
val BUILD_DIR = Path.of("pc_build")

val CFLAGS = "pkg-config --cflags sdl3".!!.strip.split(" ").toSeq
val LDFLAGS = "pkg-config --libs sdl3".!!.strip.split(" ").toSeq

def compile(sources: Set[Path], output: String) = {
  val cmd = Seq(Seq("cc", "-c"), CFLAGS, Seq("-o", output), sources.map(_.toString)).flatten(using $conforms)
  println(cmd)
  cmd.!!
}

def link(objects: Set[Path], output: String) = {
  val cmd = Seq(Seq("cc"), objects.map(_.toString), LDFLAGS, Seq("-o", output)).flatten(using $conforms)
  println(cmd)
  cmd.!!
}

def headerDeps(f: Path): Set[Path] = {
  Files.lines(f).toScala(Iterator).map(_.strip).collect {
    case s"""#include "$h"""" => headerDeps(f.getParent.resolve(h))
  }.flatten.toSet + f
}

val sources = Files.list(SOURCE_DIR).filter(_.toString.endsWith(".c")).toScala(Set)
val objects = sources.map(p => BUILD_DIR.resolve(p.getFileName.toString.replaceFirst(".c$", ".o")))

def rules(product: Path): (deps: Set[Path], fn: () => Unit) = product.toString match {
  case s"$dir/$f.o" =>
    val deps = headerDeps(SOURCE_DIR.resolve(s"$f.c"))
    (deps, () => compile(deps.filter(_.toString.endsWith(".c")), s"$dir/$f.o"))
  case EXE => (objects, () => link(objects, EXE))
  case _ => (Set(), () => ())
}

def outdated(product: Path): Boolean = {
  val mtime = Files.getLastModifiedTime(product)
  rules(product).deps.exists(d => !Files.exists(d) || Files.getLastModifiedTime(d) > mtime || outdated(d))
}

def make(product: Path): Unit = {
  if (!Files.exists(product) || outdated(product)) {
    rules(product).deps.map(make)
    rules(product).fn()
  }
}

@main
def main() = {
  if (Files.notExists(BUILD_DIR)) Files.createDirectory(BUILD_DIR)
  make(Path.of(EXE))
}
