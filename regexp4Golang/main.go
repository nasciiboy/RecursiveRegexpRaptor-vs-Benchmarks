package main

//#include <time.h>
import "C"

import (
  "fmt"
  "io/ioutil"
  "os"
  "strconv"

  "./regexp4"
)

func main(){
  if len( os.Args ) < 4 {
    fmt.Fprintf( os.Stderr, "regexp4 (Golang): wrong args" )
    os.Exit( 1 )
  }

  input, err := ioutil.ReadFile( os.Args[1] )
  if err != nil {
    fmt.Fprintf( os.Stderr, "regexp4 (Golang): %v\n", err )
    os.Exit( 1 )
  }
  data := string( input )

  n, err := strconv.Atoi( os.Args[2] )
  if err != nil {
    fmt.Fprintf( os.Stderr, "regexp4 (Golang): n arg: %v", err )
    os.Exit( 1 )
  }

  if n < 1 {
    fmt.Fprintf( os.Stderr, "regexp4 (Golang): n < 0" )
    os.Exit( 1 )
  }

  const fileOutput = "result.raptor.regexp4Golang"
  out, err := os.Create( fileOutput )
  if err != nil {
    fmt.Fprintf( os.Stderr, "regexp4 (Golang): %v\n", err )
    os.Exit( 1 )
  }
  defer out.Close()

  for _, rexp := range os.Args[3:] {
    if rexp == "---" {
      out.WriteString( "---\n" )
      continue
    }

    re := regexp4.Compile( rexp )

    var bestTime uint64
    var oneLoop  bool

    for i := 0; i < n; i++ {
      start := CpuTime()
      re.FindString( data )
      time  := DiffCpuTimeByMS( start, CpuTime() )

      if !oneLoop || time < bestTime {
        bestTime = time
        oneLoop = true
      }
    }

    srexp := saveRexp( rexp )

    fmt.Printf( "loops %d, best time %dms, matches %d, rexp %q\n", n, bestTime, re.Result(), srexp )
    out.WriteString( fmt.Sprintf( "%d %d %q\n", bestTime, re.Result(), srexp ) )
  }
}

func CpuTime() uint64 {
  return uint64(C.clock())
}

func DiffCpuTimeByMS( begin, end uint64 ) uint64 {
  return (end - begin) *  1000 / uint64(C.CLOCKS_PER_SEC)
}

func saveRexp( str string ) (r string) {
  for _, c := range str {
    switch c {
    case '\a': r += "\\a"
    case '\b': r += "\\b"
    case '\t': r += "\\t"
    case '\n': r += "\\n"
    case '\v': r += "\\v"
    case '\f': r += "\\f"
    case '\r': r += "\\r"
    case '\\': r += "\\"
    default: r += string( c )
    }
  }

  return
}
