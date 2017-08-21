package main

import (
  "os"
  "fmt"
  "io/ioutil"
  "html/template"

  "../regexp4Golang/regexp4"
)

type row struct {
  ms      string
  catches string
  rexp    string
  empty   bool
}

type fileResult struct {
  file  string
  class string
  rows  []row
}

const target = "table"

func main(){
  if len( os.Args ) < 2 {
    fmt.Fprintf( os.Stderr, "makeTable: empty imput\n" )
    os.Exit( 1 )
  }

  outFile, err := os.Create( target )
  if err != nil {
    fmt.Fprintf( os.Stderr, "makeTable: %v\n", err )
    os.Exit( 1 )
  }
  defer outFile.Close()

  outFile.WriteString( `<table class="results" border="1" width="100%">` )
  outFile.WriteString( "\n" )

  rexps   := make( map[string][]string )
  reli    := regexp4.Compile( "#^<:d+> <:d+> <.+>" )
  refi    := regexp4.Compile( "#?result:.<[^.]+>:.<.+>" )
  results := make( []fileResult, len( os.Args[1:] ) )

  for i, arg := range os.Args[1:] {
    file, err := ioutil.ReadFile( arg )
    if err != nil {
      fmt.Fprintf( os.Stderr, "makeTable: empty imput\n" )
      results[ i ].class = "empty"
      continue
    }

    lines := GetLines( string( file ) )

    refi.MatchString( arg )
    results[ i ].file  = refi.GetCatch( 2 )
    results[ i ].class = refi.GetCatch( 1 )
    results[ i ].rows  = make( []row, len( lines ) )

    for j, line := range lines {
      if line == "---" {
        results[ i ].rows[ j ].empty = true
      } else if reli.FindString( line ) {
        results[ i ].rows[ j ].ms      = reli.GetCatch( 1 )
        results[ i ].rows[ j ].catches = reli.GetCatch( 2 )
        results[ i ].rows[ j ].rexp    = reli.GetCatch( 3 )
      }
    }

    if rexps[ results[ i ].class ] == nil {
      rexps[ results[ i ].class ] = make( []string, len( results[ i ].rows ) )
      for k, r := range results[ i ].rows {
        rexps[ results[ i ].class ][k] = r.rexp
      }
    }
  }

  pclass, pelements := "", 0
  for key, res := range rexps {
    if pclass == "" {
      pelements = len( res )
      pclass    = key
    } else if pelements != len( res ) {
      fmt.Fprintf( os.Stderr, "makeTable: input data\n" )
      fmt.Fprintf( os.Stderr, "  test: %q has %d elements\n", pclass, pelements )
      fmt.Fprintf( os.Stderr, "  test: %q has %d elements\n", key, len( res ) )
      os.Exit( 1 )
    }
  }

  outFile.WriteString( "<thead>\n" )
  outFile.WriteString( "<tr><th>Regular expression</th>" )
  for i := range results {
    outFile.WriteString( fmt.Sprintf( "<th class=%q class=%q >%s</th>", results[i].class, results[i].class, results[i].file ) )
  }
  outFile.WriteString( "</tr>\n</thead>\n" )


  outFile.WriteString( "<tbody>\n" )
  for y := range results[0].rows {
    outFile.WriteString( `<tr><td class="pattern" ><table>` )
    for key, res := range rexps {
      outFile.WriteString( fmt.Sprintf( "<tr class=%q ><td>%s</tr></td>", key, template.HTMLEscapeString( res[ y ] )  ) )
    }
    outFile.WriteString( `</table></td>` )

    for x := range results {
      if results[x].rows[ y ].empty {
        outFile.WriteString( fmt.Sprintf( "<td class=%q class=%q class=\"tbody\" ><b>N/A</b></td>", results[x].file, results[x].class ) )
      } else {
        outFile.WriteString( fmt.Sprintf( "<td class=%q class=%q class=\"tbody\" >%sms (%s)</td>", results[x].file, results[x].class, results[x].rows[ y ].ms, results[x].rows[ y ].catches ) )
      }
    }

    outFile.WriteString( "</tr>\n" )
  }
  outFile.WriteString( "</tbody>\n" )

  outFile.WriteString( "</table>\n" )
  fmt.Fprintf( os.Stdout, "makeTable: The table was created successfully, see %q file\n", target )
}


func GetLines( str string ) (result []string) {
  result = make( []string, 0, 64 )
  last := 0;
  for i, c := range str {
    if c == '\n' {
      result = append( result, str[last:i] )
      last = i + 1
    }
  }

  if last < len( str ) { result = append( result, str[last:] ) }

  return result
}
