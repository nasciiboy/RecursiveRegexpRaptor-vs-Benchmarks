#include "regexp3.h"
#include "charUtils.h"

#define TRUE                 1
#define FALSE                0
#define MAX_CATCHS     1000000
#define INF         1073741824 // 2^30

#define MOD_ALPHA            1
#define MOD_OMEGA            2
#define MOD_LONLEY           4
#define MOD_FwrByChar        8
#define MOD_COMMUNISM       16

struct CATch {
  char *ptr[ MAX_CATCHS ];
  int   len[ MAX_CATCHS ];
  int   id [ MAX_CATCHS ];
  int   idx;
  int   index;
} static Catch;

struct TEXT {
  char *ptr;
  int   pos;
  int   len;
} static text;

enum TYPE { PATH, GROUP, HOOK, BRACKET, BACKREF, META, RANGEAB, UTF8, POINT, SIMPLE };

struct RE {
  char          *ptr;
  int            len;
  enum     TYPE  type;
  unsigned char  mods;
  unsigned int   loopsMin, loopsMax;
};

static int  walker       ( struct RE  rexp );
static int  trekking     ( struct RE *rexp );
static int  looper       ( struct RE *rexp );
static int  tracker      ( struct RE *rexp, struct RE *track );
static int  cutTrack     ( struct RE *rexp, struct RE *track, int type );

static void trackByLen   ( struct RE *rexp, struct RE *track, int len, int type );
static void getMods      ( struct RE *rexp, struct RE *track );
static void setLoops     ( struct RE *rexp, struct RE *track );
static void fwrTrack     ( struct RE *track, int len );
static int  walkMeta     ( char *str );
static int  walkBracket  ( char *str );
static char *trackerPoint( char *track, int len );

static int  match        ( struct RE *rexp );
static int  matchBracket ( struct RE  rexp );
static int  matchBackRef ( struct RE *rexp );
static int  matchRange   ( struct RE *rexp, int   chr );
static int  matchMeta    ( struct RE *rexp, char *txt );
static int  matchText    ( struct RE *rexp, char *txt );

static void openCatch    ( int *index );
static void closeCatch   ( int  index );
static int  lastIdCatch  ( int  id    );

int regexp3( char *txt, char *re, int txtLen ){
  struct RE    rexp;
  int result   = 0;
  text.len     = txtLen; //strLen( txt );
  Catch.ptr[0] = txt;
  Catch.len[0] = text.len;
  Catch.id [0] = 0;
  Catch.index  = 1;
  rexp.ptr     = re;
  rexp.type    = PATH;
  rexp.len     = strLen( re );
  rexp.mods    = 0;

  if( text.len == 0 || rexp.len == 0 ) return 0;

  getMods( &rexp, &rexp );

  for( int forward, i = 0, loops = rexp.mods & MOD_ALPHA ? 1 : text.len; i < loops; i += forward ){
    forward    = utf8meter( txt + i );
    Catch.idx  = 1;
    text.pos   = 0;
    text.ptr   = txt   + i;
    text.len   = Catch.len[0] - i;

    if( walker( rexp ) ){
      if     (  rexp.mods & MOD_OMEGA    ){ if( text.pos == text.len ) return TRUE; }
      else if(  rexp.mods & MOD_LONLEY   )                             return TRUE;
      else if( (rexp.mods & MOD_FwrByChar) || text.pos == 0 )          result++;
      else   {  forward = text.pos;                                    result++; }
    }
  }

  return result;
}

static int walker( struct RE rexp ){
  struct RE track;
  for( const int oCindex = Catch.index, oCidx = Catch.idx, oTpos = text.pos;
       cutTrack( &rexp, &track, PATH );
       Catch.index = oCindex, Catch.idx = oCidx, text.pos = oTpos )
    if( track.len && trekking( &track ) ) return TRUE;

  return FALSE;
}

static int trekking( struct RE *rexp ){
  struct RE track;
  while( tracker( rexp, &track ) )
    if( track.len == 0 || looper( &track ) == FALSE ) return FALSE;

  return TRUE;
}

static int looper( struct RE *rexp ){
  int iCatch, steps, loops = 0;

  switch( rexp->type ){
  case HOOK:
    openCatch( &iCatch );
    while( loops < rexp->loopsMax && walker( *rexp ) )
      loops++;
    if( loops >= rexp->loopsMin ) closeCatch( iCatch );
    break;
  case GROUP: case PATH:
    while( loops < rexp->loopsMax && walker( *rexp ) )
      loops++;
    break;
  case BRACKET: case BACKREF: case META: case RANGEAB: case UTF8: case POINT: case SIMPLE:
    while( loops < rexp->loopsMax && text.pos < text.len && (steps = match( rexp )) ){
      text.pos += steps;
      loops++;
    }
  }

  return loops < rexp->loopsMin ? FALSE : TRUE;
}

static int cutTrack( struct RE *rexp, struct RE *track, int type ){
  if( rexp->len == 0 ) return FALSE;

  *track      = *rexp;
  track->type = type;
  if( type != PATH ) fwrTrack( track, 1 );

  for( int cut, i = 0, deep = 0; i < rexp->len; i++ ){
    i += walkMeta( rexp->ptr + i );

    switch( rexp->ptr[i] ){
    case '<': case '(': deep++; break;
    case '>': case ')': deep--; break;
    case '[': i += walkBracket( rexp->ptr + i ); break;
    }

    switch( type ){
    case HOOK    : cut = deep == 0; break;
    case GROUP   : cut = deep == 0; break;
    case BRACKET : cut =              rexp->ptr[i] == ']'; break;
    case PATH    : cut = deep == 0 && rexp->ptr[i] == '|'; break;
    }

    if( cut ){
      track->len  = &rexp->ptr[i] - track->ptr;
      fwrTrack( rexp, i + 1 );
      return TRUE;
    }
  }

  fwrTrack( rexp, rexp->len );
  return TRUE;
}

static int tracker( struct RE *rexp, struct RE *track ){
  char *point;

  if( rexp->len ){
    switch( *rexp->ptr & xooooooo ? UTF8 : *rexp->ptr ){
    case ':' : trackByLen( rexp, track, 2,                    META    ); break;
    case '.' : trackByLen( rexp, track, 1,                    POINT   ); break;
    case '@' : trackByLen( rexp, track, 1 +
                                      countCharDigits( rexp->ptr + 1 ),
                                                              BACKREF ); break;
    case '(' : cutTrack  ( rexp, track,                       GROUP   ); break;
    case '<' : cutTrack  ( rexp, track,                       HOOK    ); break;
    case '[' : cutTrack  ( rexp, track,                       BRACKET ); break;
    case UTF8: trackByLen( rexp, track, utf8meter(rexp->ptr), UTF8    ); break;
    default :
      if( (point = trackerPoint( rexp->ptr + 1, rexp->len - 1 )) ){
        switch( *point ){
        default: trackByLen( rexp, track, point - rexp->ptr, SIMPLE  ); break;
        case '?': case '+': case '*': case '{': case '-': case '#':
          if( point - rexp->ptr == 1 ){
            if( *point == '-' ) trackByLen( rexp, track, 3, RANGEAB );
            else                trackByLen( rexp, track, 1, SIMPLE  );
          } else trackByLen( rexp, track, (point - rexp->ptr) - 1, SIMPLE  );
        }
      } else trackByLen( rexp, track, rexp->len, SIMPLE  );
    }

    setLoops( rexp, track );
    getMods ( rexp, track );
    return TRUE;
  }

  return FALSE;
}

static void trackByLen( struct RE *rexp, struct RE *track, int len, int type ){
  *track       = *rexp;
  track->type  = type;
  track->len   = len;
  fwrTrack( rexp, len );
}

static char * trackerPoint( char *track, int len ){
  for( int pos = 0; pos < len; pos++ )
    if( strChr( "(<[@:.?+*{-#", track[ pos ] ) || track[ pos ] & xooooooo ) return track + pos;

  return 0;
}

static int walkBracket( char *str ){
  int i = 0;
  while( TRUE )
    switch( str[ i ] ){
    case ']': return i;
    case ':': i += 2; break;
    default : i++   ; break;
    }
}

static int walkMeta( char *str ){
  for( int i = 0; ; i += 2 )
    if( str[i] != ':' ) return i;
}

static void fwrTrack( struct RE *track, int len ){
  track->ptr += len; track->len -= len;
}

static void getMods( struct RE *rexp, struct RE *track ){
  int inMods = *rexp->ptr == '#', pos = 0;

  while( inMods )
    switch( rexp->ptr[ ++pos ] ){
    case '^': track->mods |=  MOD_ALPHA     ; break;
    case '$': track->mods |=  MOD_OMEGA     ; break;
    case '?': track->mods |=  MOD_LONLEY    ; break;
    case '~': track->mods |=  MOD_FwrByChar ; break;
    case '*': track->mods |=  MOD_COMMUNISM ; break;
    case '/': track->mods &= ~MOD_COMMUNISM ; break;
    default : inMods       =  FALSE         ; break;
    }

  fwrTrack( rexp, pos );
}

static void setLoops( struct RE *rexp, struct RE *track ){
  track->loopsMin = 1; track->loopsMax = 1;
  int len = 0;

  if( rexp->len )
    switch( *rexp->ptr ){
    case '?' : len = 1; track->loopsMin = 0; track->loopsMax =   1; break;
    case '+' : len = 1; track->loopsMin = 1; track->loopsMax = INF; break;
    case '*' : len = 1; track->loopsMin = 0; track->loopsMax = INF; break;
    case '{' :
      track->loopsMin = aToi( rexp->ptr + 1 ) ;
      if( rexp->ptr[ 1 + countCharDigits( rexp->ptr + 1 ) ] == ',' )
        track->loopsMax = aToi( strChr( rexp->ptr, ',' ) + 1 );
      else
        track->loopsMax = track->loopsMin;

      len = strChr( rexp->ptr, '}' ) - rexp->ptr + 1;
    }

  fwrTrack( rexp, len );
}

static int match( struct RE *rexp ){
  switch( rexp->type ){
  case POINT  : return utf8meter( text.ptr + text.pos );
  case BRACKET: return matchBracket( *rexp );
  case BACKREF: return matchBackRef(  rexp );
  case RANGEAB: return matchRange( rexp, text.ptr[text.pos]  );
  case META   : return matchMeta ( rexp, text.ptr + text.pos );
  default     : return matchText ( rexp, text.ptr + text.pos );
  }
}

static int matchText( struct RE *rexp, char *txt ){
  if( rexp->mods & MOD_COMMUNISM )
    return    strnEqlCommunist( txt, rexp->ptr, rexp->len ) ? rexp->len : 0;
  else return strnEql         ( txt, rexp->ptr, rexp->len ) ? rexp->len : 0;
}

static int matchRange( struct RE *rexp, int chr ){
  if( rexp->mods & MOD_COMMUNISM ){
    chr = toLower( chr );
    return chr >= toLower( rexp->ptr[ 0 ] ) && chr <= toLower( rexp->ptr[ 2 ] );
  } else
    return chr >=          rexp->ptr[ 0 ]   && chr <=          rexp->ptr[ 2 ];
}

static int matchMeta( struct RE *rexp, char *txt ){
  switch( rexp->ptr[1] ){
  case 'a' : return  isAlpha( *txt );
  case 'A' : return !isAlpha( *txt ) ? utf8meter( txt ) : FALSE;
  case 'd' : return  isDigit( *txt );
  case 'D' : return !isDigit( *txt ) ? utf8meter( txt ) : FALSE;
  case 'w' : return  isAlnum( *txt );
  case 'W' : return !isAlnum( *txt ) ? utf8meter( txt ) : FALSE;
  case 's' : return  isSpace( *txt );
  case 'S' : return !isSpace( *txt ) ? utf8meter( txt ) : FALSE;
  case '&' : return *txt & xooooooo  ? utf8meter( txt ) : FALSE;
  default  : return *txt == rexp->ptr[1];
  }
}

static int matchBracket( struct RE rexp ){
  struct RE track;
  int result  = 0, reverse = *rexp.ptr == '^';

  if( reverse ) fwrTrack( &rexp, 1 );

  while( tracker( &rexp, &track ) ){
    switch( track.type ){
    case GROUP:
      result = walker( track ); break;
    case RANGEAB: case META: case UTF8: case POINT:
      result = match( &track ); break;
    default:
      if( track.mods & MOD_COMMUNISM )
           result = strnChrCommunist( track.ptr, text.ptr[ text.pos ], track.len  ) != 0;
      else result = strnChr         ( track.ptr, text.ptr[ text.pos ], track.len  ) != 0;
    }

    if( result ) return reverse ? FALSE : result;
  }

  return reverse ? utf8meter( text.ptr + text.pos ) : FALSE;
}

static int matchBackRef( struct RE *rexp ){
  int backRefId    = aToi( rexp->ptr + 1 );
  int backRefIndex = lastIdCatch( backRefId );
  if( gpsCatch( backRefIndex ) == 0 ||
      strnEql( text.ptr + text.pos, gpsCatch( backRefIndex ), lenCatch( backRefIndex ) ) == FALSE )
    return FALSE;
  else return lenCatch( backRefIndex );
}

static void openCatch( int *index ){
  if( Catch.index < MAX_CATCHS ){
    *index = Catch.index++;
    Catch.ptr[ *index ] = text.ptr + text.pos;
    Catch.id [ *index ] = Catch.idx++;
  }
}

static void closeCatch( int index ){
  if( index < MAX_CATCHS )
    Catch.len[ index ] = &text.ptr[ text.pos ] - Catch.ptr[ index ];
}

static int lastIdCatch( int id ){
  int lastId = MAX_CATCHS;
  for( int index = 1; index < Catch.index; index++ )
    if( Catch.id[ index ] == id ) lastId = index;

  return lastId;
}

int totalCatch(){ return Catch.index - 1; }

char * gpsCatch( int index ){
  return ( index > 0 && index < Catch.index ) ? Catch.ptr[ index ] : 0;
}

int lenCatch( int index ){
  return ( index > 0 && index < Catch.index ) ? Catch.len[ index ] : 0;
}

char * cpyCatch( char * str, int index ){
  if( index > 0 && index < Catch.index )
    strnCpy( str, Catch.ptr[ index ], Catch.len[ index ] );
  else *str = '\0';

  return str;
}

char * rplCatch( char * newStr, char * rplStr, int id ){
  char *oNewStr = newStr, *text = Catch.ptr[ 0 ];
  strCpy( newStr, text );

  for( int index = 1; index < Catch.index; index++ )
    if( Catch.id[ index ] == id ){
      newStr += Catch.ptr[ index ] - text;
      strCpy( newStr, rplStr );
      newStr += strLen( rplStr );
      text    = Catch.ptr[ index ] + Catch.len[ index ];
      strCpy( newStr, text );
    }

  return oNewStr;
}

char * putCatch( char * newStr, char * putStr ){
  int  index; char *pos, *oNewStr = newStr;
  strCpy( newStr, putStr );

  while( (pos = strChr( putStr, '#' )) ){
    if( pos[ 1 ] == '#' ){
      newStr += pos + 1 - putStr;
      putStr  = pos + 2;
    } else {
      index   = aToi( pos + 1 );
      newStr += pos - putStr;
      cpyCatch( newStr, index );
      newStr += lenCatch( index );
      putStr  = pos + 1 + countCharDigits( pos + 1 );
    }

    strCpy( newStr, putStr );
  }

  return oNewStr;
}
