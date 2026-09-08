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

def compile(sources: Set[String], output: String): Boolean = {
  val cmd = Seq(Seq("cc", "-c"), CFLAGS, Seq("-o", output), sources).flatten(using $conforms)
  println(cmd)
  cmd.! == 0
}

def link(objects: Set[String], output: String): Boolean = {
  val cmd = Seq(Seq("cc"), objects, LDFLAGS, Seq("-o", output)).flatten(using $conforms)
  println(cmd)
  cmd.! == 0
}

def headerDeps(f: Path): Set[Path] = {
  Files.lines(f).toScala(Iterator).map(_.strip).collect {
    case s"""#include "$h"""" => headerDeps(f.getParent.resolve(h))
  }.flatten.toSet + f
}

val sources = Files.list(SOURCE_DIR).filter(_.toString.endsWith(".c")).toScala(Set)
val objects = sources.map(p => BUILD_DIR.resolve(p.getFileName.toString.replaceFirst(".c$", ".o")).toString)

val rules: PartialFunction[String, (Set[String], () => Boolean)] = {
  case s"$dir/$f.o" =>
    val deps = headerDeps(SOURCE_DIR.resolve(s"$f.c")).map(_.toString)
    (deps, () => compile(deps.filter(_.endsWith(".c")), s"$dir/$f.o"))
  case EXE => (objects, () => link(objects, EXE))
  case _ => (Set(), () => false)
}

def outdated(f: String): Boolean = {
  val mtime = Files.getLastModifiedTime(Path.of(f))
  rules(f)._1.exists(d => !Files.exists(Path.of(d)) || Files.getLastModifiedTime(Path.of(d)) > mtime || outdated(d))
}

def make(f: String): Unit = {
  if (!Files.exists(Path.of(f)) || outdated(f)) {
    rules(f)._1.map(make)
    if (!rules(f)._2()) throw Exception()
  }
}

@main
def main() = make(EXE)
