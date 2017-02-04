#ifndef REGEXP4_H
#define REGEXP4_H
/*************************************************************************
 * <Recursive Regexp Raptor (regexp4) regexp library.>
 * https://github.com/nasciiboy/RecursiveRegexpRaptor-4
 * Copyright (C) <2016>  <nasciiboy>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

int  regexp4( const char *txt, const char *re, const int txtLen );

const
char *gpsCatch_4( const int index );
int   totCatch_4();
int   lenCatch_4( const int index );
char *cpyCatch_4( char *str, const int index );
char *rplCatch_4( char *newStr, const char *rplStr, const int id );
char *putCatch_4( char *newStr, const char *putStr );

#endif
