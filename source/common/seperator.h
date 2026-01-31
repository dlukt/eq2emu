/*  
    EQ2Emulator:  Everquest II Server Emulator
    Copyright (C) 2007  EQ2EMulator Development Team (http://www.eq2emulator.net)

    This file is part of EQ2Emulator.

    EQ2Emulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EQ2Emulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EQ2Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/
// This class will split up a string smartly at the div character (default is space and tab)
// Seperator.arg[i] is a copy of the string chopped at the divs
// Seperator.argplus[i] is a pointer to the original string so it doesnt end at the div

// Written by Quagmire
#ifndef SEPERATOR_H
#define SEPERATOR_H

#include <string.h>
#include <stdlib.h>

class Seperator
{
public:
	Seperator(const char* message, char div = ' ', int16 in_maxargnum = 10, int16 arglen = 100, bool iObeyQuotes = false, char div2 = '\t', char div3 = 0, bool iSkipEmpty = true) {
		int i;
		argnum = 0;
		msg = strdup(message);
		this->maxargnum = in_maxargnum;
		argplus = new const char *[maxargnum+1];
		arg = new char *[maxargnum+1];

		// Bolt: Optimized to single allocation for all arguments
		// This reduces N+1 allocations to 1, significantly improving performance for command parsing
		int32_t total_buffer_size = (maxargnum + 1) * (arglen + 1);
		arg_buffer = new char[total_buffer_size];
		memset(arg_buffer, 0, total_buffer_size);

		for (i=0; i<=maxargnum; i++) {
			argplus[i]=arg[i] = &arg_buffer[i * (arglen + 1)];
		}

		int s = 0, l = 0;
		bool inarg = (!iSkipEmpty || !(message[0] == div || message[0] == div2 || message[0] == div3));
		bool inquote = (iObeyQuotes && (message[0] == '\"' || message[0] == '\''));
		argplus[0] = message;
		if (message[0] == '\0')
			return;

		for (i=0; message[i] != '\0'; i++) {
//			cout << i << ": 0x" << hex << (int) message[i] << dec << " " << message[i] << endl;
			if (inarg) {
				if ((inquote == false && (message[i] == div || message[i] == div2 || message[i] == div3)) || (inquote && (message[i] == '\'' || message[i] == '\"') && (message[i+1] == div || message[i+1] == div2 || message[i+1] == div3 || message[i+1] == 0))) {
					inquote = false;					
					l = i-s;					
					if (l >= arglen)
						l = arglen;
					if (l){
						if(l > 1 && (argplus[argnum][0] == '\'' || argplus[argnum][0] == '\"')){
							l--;
							memcpy(arg[argnum], argplus[argnum]+1, l);
						}
						else
							memcpy(arg[argnum], argplus[argnum], l);
					}
					arg[argnum][l] = 0;
					argnum++;
					if (iSkipEmpty)
						inarg = false;
					else {
						s=i+1;
						argplus[argnum] = &message[s];
					}
				}
			}
			else if (iObeyQuotes && (message[i] == '\"' || message[i] == '\'')) {
				inquote = true;
			}
			else {
				s = i;
				argplus[argnum] = &message[s];
				if (!(message[i] == div || message[i] == div2 || message[i] == div3)) {
					inarg = true;
				}
			}
			if (argnum > maxargnum)
				break;
		}
		if (inarg && argnum <= maxargnum) {
			l = i-s;
			if (l >= arglen)
				l = arglen;
			if (l)
				memcpy(arg[argnum], argplus[argnum], l);
		}
	}
	~Seperator() {
		// Bolt: Delete the single buffer instead of individual args
		safe_delete_array(arg_buffer);

		safe_delete_array(arg);
		safe_delete_array(argplus);
		if (msg)
			free(msg);
	}
	int16 argnum;
	char** arg;
	const char** argplus;
	char * msg;
	bool IsSet(int num) const {
		return IsSet(arg[num]);
	}
	bool IsNumber(int num) const {
		return IsNumber(arg[num]);
	}
	// Bolt: Optimized integer parsing to avoid double-scanning strings
	// Returns true if valid number and populates out_val
	bool GetInt(int index, int32& out_val) const {
		if (index < 0 || index > argnum) return false;
		return GetInt(arg[index], out_val);
	}
	bool GetUInt(int index, uint32& out_val) const {
		if (index < 0 || index > argnum) return false;
		return GetUInt(arg[index], out_val);
	}
	bool IsHexNumber(int num) const {
		return IsHexNumber(arg[num]);
	}
	static bool IsSet(const char *check) {
		return check[0] != '\0';
	}
	static bool IsNumber(const char* check) {
		if (!check || !*check)
			return false;

		bool SeenDec = false;
		const char* p = check;

		// Handle optional sign at the beginning
		if (*p == '-' || *p == '+') {
			p++;
			// Sign alone is not a number
			if (!*p)
				return false;
		}

		for (; *p; ++p) {
			if (*p < '0' || *p > '9') {
				if (*p == '.' && !SeenDec) {
					SeenDec = true;
				}
				else {
					return false;
				}
			}
		}
		return true;
	}

	static bool GetInt(const char* check, int32& out_val) {
		if (!check || !*check) return false;

		char* end;
		// strtol is base 10 here
		long val = strtol(check, &end, 10);

		if (check == end) return false;

		if (*end != '\0') {
			if (*end == '.') {
				const char* p = end + 1;
				while (*p) {
					if (*p < '0' || *p > '9') return false;
					p++;
				}
			} else {
				return false;
			}
		}

		out_val = (int32)val;
		return true;
	}
	static bool GetUInt(const char* check, uint32& out_val) {
		if (!check || !*check) return false;

		char* end;
		unsigned long val = strtoul(check, &end, 10);

		if (check == end) return false;

		if (*end != '\0') {
			if (*end == '.') {
				const char* p = end + 1;
				while (*p) {
					if (*p < '0' || *p > '9') return false;
					p++;
				}
			} else {
				return false;
			}
		}

		out_val = (uint32)val;
		return true;
	}
	static bool IsHexNumber(const char* check) {
		if (!check || check[0] != '0' || (check[1] != 'x' && check[1] != 'X'))
			return false;

		const char* p = check + 2;
		if (!*p) return false; // "0x" is not a number? original logic was len < 3 so yes.

		for (; *p; ++p) {
			if ((*p < '0' || *p > '9') && (*p < 'A' || *p > 'F') && (*p < 'a' || *p > 'f'))
				return false;
		}
		return true;
	}
	inline int16 GetMaxArgNum() const { return maxargnum; }
	inline int16 GetArgNumber() const { return argnum; }
private:
	int16 maxargnum;
	char* arg_buffer; // Bolt: Single buffer for all args
};

#endif
