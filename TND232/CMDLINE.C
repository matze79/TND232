
#line 1 "cmdline.rl"
#include <stddef.h>
#include "resident.h"
#include "cmdline.h"


#line 30 "cmdline.rl"



enum mode parse_command_line (char _WCI86FAR *p)
{
  enum mode mode;
  int cs;
  
#line 13 "cmdline.c"
static const char _cmdline_key_offsets[] = {
	0, 0, 11, 13, 15, 17, 20, 27, 
	29, 31, 33, 35, 37, 39, 41, 43, 
	45, 48, 50, 52, 54, 56, 58
};

static const char _cmdline_trans_keys[] = {
	0, 13, 32, 67, 68, 83, 85, 99, 
	100, 115, 117, 79, 111, 77, 109, 49, 
	52, 0, 13, 32, 0, 13, 32, 67, 
	68, 99, 100, 69, 101, 66, 98, 85, 
	117, 71, 103, 84, 116, 65, 97, 84, 
	116, 85, 117, 83, 115, 0, 13, 32, 
	78, 110, 76, 108, 79, 111, 65, 97, 
	68, 100, 0
};

static const char _cmdline_single_lengths[] = {
	0, 11, 2, 2, 0, 3, 7, 2, 
	2, 2, 2, 2, 2, 2, 2, 2, 
	3, 2, 2, 2, 2, 2, 0
};

static const char _cmdline_range_lengths[] = {
	0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _cmdline_index_offsets[] = {
	0, 0, 12, 15, 18, 20, 24, 32, 
	35, 38, 41, 44, 47, 50, 53, 56, 
	59, 63, 66, 69, 72, 75, 78
};

static const char _cmdline_indicies[] = {
	0, 0, 2, 3, 4, 5, 6, 3, 
	4, 5, 6, 1, 7, 7, 1, 8, 
	8, 1, 9, 1, 0, 0, 10, 1, 
	11, 11, 12, 3, 4, 3, 4, 1, 
	13, 13, 1, 14, 14, 1, 15, 15, 
	1, 16, 16, 1, 17, 17, 1, 18, 
	18, 1, 19, 19, 1, 20, 20, 1, 
	21, 21, 1, 11, 11, 22, 1, 23, 
	23, 1, 24, 24, 1, 25, 25, 1, 
	26, 26, 1, 27, 27, 1, 1, 0
};

static const char _cmdline_trans_targs[] = {
	22, 0, 1, 2, 7, 11, 17, 3, 
	4, 5, 6, 22, 6, 8, 9, 10, 
	5, 12, 13, 14, 15, 16, 16, 18, 
	19, 20, 21, 16
};

static const char _cmdline_trans_actions[] = {
	1, 0, 2, 0, 0, 0, 0, 0, 
	0, 3, 2, 4, 0, 0, 0, 0, 
	5, 0, 0, 0, 0, 6, 0, 0, 
	0, 0, 0, 7
};

static const int cmdline_start = 1;
static const int cmdline_first_final = 22;
static const int cmdline_error = 0;

static const int cmdline_en_main = 1;


#line 38 "cmdline.rl"
  
#line 81 "cmdline.c"
	{
	cs = cmdline_start;
	}

#line 39 "cmdline.rl"
  
#line 84 "cmdline.c"
	{
	int _klen;
	const char *_keys;
	int _trans;

	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _cmdline_trans_keys + _cmdline_key_offsets[cs];
	_trans = _cmdline_index_offsets[cs];

	_klen = _cmdline_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _cmdline_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _cmdline_indicies[_trans];
	cs = _cmdline_trans_targs[_trans];

	if ( _cmdline_trans_actions[_trans] == 0 )
		goto _again;

	switch ( _cmdline_trans_actions[_trans] ) {
	case 2:
#line 9 "cmdline.rl"
	{ mode = MODE_LOAD; }
	break;
	case 7:
#line 10 "cmdline.rl"
	{ mode = MODE_UNLOAD; }
	break;
	case 6:
#line 11 "cmdline.rl"
	{ mode = MODE_STATUS; }
	break;
	case 3:
#line 12 "cmdline.rl"
	{ config.bios_id = *p - '1'; }
	break;
	case 5:
#line 13 "cmdline.rl"
	{ config.debug = 1; }
	break;
	case 4:
#line 16 "cmdline.rl"
	{ {p++; goto _out; } }
	break;
	case 1:
#line 9 "cmdline.rl"
	{ mode = MODE_LOAD; }
#line 16 "cmdline.rl"
	{ {p++; goto _out; } }
	break;
#line 170 "cmdline.c"
	}

_again:
	if ( cs == 0 )
		goto _out;
	p += 1;
	goto _resume;
	_out: {}
	}

#line 40 "cmdline.rl"
  if (cs < cmdline_first_final) {
    mode = MODE_USAGE;
  }
  return mode;
}
