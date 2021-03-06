/*===============*\
| FILE I/O SOURCE |
\*===============*/



#include "main.h"
#include "global.h"
#include "fileio.h"

bool xyFileExists(const char* file){
	//Checks if a file exists
	struct stat buff;
	if (stat(file, &buff) != -1) return true;

	return false;
};

// Credit to Nova Storm for the JSON encoding and decoding functions

void sqDecodeJSONTable(HSQUIRRELVM v, cJSON *Item){
  if(!Item)
    return;
  while(Item) {
    if(Item->string)
      sq_pushstring(v, Item->string, -1);
    switch(Item->type) {
      case cJSON_False:
        sq_pushbool(v, SQFalse);
        break;
      case cJSON_True:
        sq_pushbool(v, SQTrue);
        break;
      case cJSON_NULL:
        sq_pushnull(v);
        break;
      case cJSON_Number:
        if(Item->valueint == Item->valuedouble)
          sq_pushinteger(v, Item->valueint);
        else
          sq_pushfloat(v, Item->valuedouble);
        break;
      case cJSON_String:
        sq_pushstring(v, Item->valuestring, -1);
        break;
      case cJSON_Array:
        sq_newarray(v, 0);
        sqDecodeJSONTable(v, Item->child);
        break;
      case cJSON_Object:
        sq_newtable(v);
        sqDecodeJSONTable(v, Item->child);
        break;
    }
    if(Item->string)
      sq_newslot(v,-3,SQFalse);
    else
      sq_arrayappend(v, -2);
    Item = Item->next;
  }
};

SQInteger sqDecodeJSON(HSQUIRRELVM v){
  const SQChar *Str;
  sq_getstring(v, 2, &Str);
  if(Str[0]!='{' && Str[0]!='[') {
    if(!strcmp(Str, "true"))
      sq_pushbool(v, SQTrue);
    else if(!strcmp(Str, "false"))
      sq_pushbool(v, SQFalse);
    else if(isdigit(Str[0]) || (Str[0]=='-' && isdigit(Str[1])))
      sq_pushinteger(v, strtol(Str, NULL, 0));
    else
      sq_pushstring(v, Str, -1);
    return 1;
  }

  cJSON *Root = cJSON_Parse(Str);
  if(!Root || !Root->child) {
    sq_pushnull(v);
    return 1;
  }
  sq_newtable(v);
  sqDecodeJSONTable(v, Root->child);
  cJSON_Delete(Root);
  return 1;
}
