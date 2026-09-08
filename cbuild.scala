#!/usr/bin/env -S scala-cli shebang

//> using scala 3.8.4
//> using jvm 21

import java.nio.file.{Path, Files}
import scala.jdk.StreamConverters.*
import scala.sys.process.*
import scala.math.Ordering.Implicits.infixOrderingOps

val CC = "cc"

val EXE = "portal2d"

val SOURCE_DIR = Path.of("src")
val BUILD_DIR = Path.of("pc_build")

val CFLAGS = "pkg-config --cflags sdl3".!!.strip.split(" ").toSeq
val LDFLAGS = "pkg-config --libs sdl3".!!.strip.split(" ").toSeq

def headerDeps(f: Path): Set[Path] = {
  Files.lines(f).toScala(Iterator).map(_.strip).collect {
    case s"""#include "$h"""" => headerDeps(f.getParent.resolve(h))
  }.flatten.toSet + f
}

val sources = Files.list(SOURCE_DIR).filter(_.toString.endsWith(".c")).toScala(Set)
val objects = sources.map(p => BUILD_DIR.resolve(p.getFileName.toString.replaceFirst(".c$", ".o")).toString)

val deps: PartialFunction[String, Set[String]] = {
  case EXE => objects
  case s"$dir/$f.o" => headerDeps(SOURCE_DIR.resolve(s"$f.c")).map(_.toString)
  case _ => Set()
}

val rules: PartialFunction[String, Set[String] => Boolean] = {
  case f if f.endsWith(".o") => deps => {
    val cmd = Seq(Seq(CC, "-c"), CFLAGS, Seq("-o", f), deps.filter(_.endsWith(".c"))).flatten(using $conforms)
    println(cmd)
    cmd.! == 0
  }
  case EXE => objs => {
    val cmd = Seq(Seq(CC), objs, LDFLAGS, Seq("-o", EXE)).flatten(using $conforms)
    println(cmd)
    cmd.! == 0
  }
}

def outdated(f: String): Boolean = {
  val mtime = Files.getLastModifiedTime(Path.of(f))
  deps(f).exists(d => !Files.exists(Path.of(d)) || Files.getLastModifiedTime(Path.of(d)) > mtime || outdated(d))
}

def make(f: String): Unit = {
  if (!Files.exists(Path.of(f)) || outdated(f)) {
    val _deps = deps(f)
    _deps.map(make)
    if (!rules(f)(_deps)) throw Exception()
  }
}

@main
def main() = make(EXE)
