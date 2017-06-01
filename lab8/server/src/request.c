#include <string.h>
#include <ctype.h>
#include <jansson.h>
#include <time.h>

#include <request.h>

/**
    @struct List
    @brief defines a generic List data type
*/

struct Memes{
    char name[10];
    int year;
    int id;
};
 Memes me[5] ={
        {"Papich", 2011, 1},
        {"Rage faces", 2007, 2},
        {"TrollFace", 2007, 3},
        {"Me Gusta", 2008, 4},
        {"ForAlone", 2008, 5}
        };


void req1(NetMessage * message){
    time_t t;
    t=time(NULL);
    json_t * jobj = json_object();
    json_object_set_new(jobj, "title", json_string("MEMES"));
    json_object_set_new(jobj, "developer", json_string("Nikita Maslov"));
    json_object_set_new(jobj, "time", json_string(ctime(&t)));
    const char * strDump = json_dumps(jobj, JSON_COMPACT);
	NetMessage_setDataString(message, strDump);
	free((void *)strDump);
	json_decref(jobj);
}
void reqFavorites(NetMessage * message){
    json_t * ar = json_array();
    for(int i = 0; i < 5; i++){
        json_t * jobj = json_object();
        json_object_set_new(jobj, "name", json_string(me[i].name));
        json_object_set_new(jobj, "year", json_integer(me[i].year));
        json_object_set_new(jobj, "id", json_integer(me[i].id));
        json_array_append(ar, jobj);
        json_decref(jobj);
    }
    const char * strDump = json_dumps(ar, JSON_COMPACT);
	NetMessage_setDataString(message, strDump);
	free((void *)strDump);
	json_decref(ar);
}
void reqFavoritesKey(int key, char* value, NetMessage * message){
    json_t * ar = json_array();
    int j = 0;
    for(int i = 0; i < 5; i++){
        if(key == 0){
            if(strcmp(me[i].name, value)==0){
                json_t * jobj = json_object();
                json_object_set_new(jobj, "name", json_string(me[i].name));
                json_object_set_new(jobj, "year", json_integer(me[i].year));
                json_object_set_new(jobj, "id", json_integer(me[i].id));
                json_array_append(ar, jobj);
                json_decref(jobj);
                j++;
            }
        }
        else if(key == 1){
            if(me[i].year == atoi(value)){
                json_t * jobj = json_object();
                json_object_set_new(jobj, "name", json_string(me[i].name));
                json_object_set_new(jobj, "year", json_integer(me[i].year));
                json_object_set_new(jobj, "id", json_integer(me[i].id));
                json_array_append(ar, jobj);
                json_decref(jobj);
                j++;
            }
        }
        else if(key == 2){
            if(me[i].id == atoi(value)){
                json_t * jobj = json_object();
                json_object_set_new(jobj, "name", json_string(me[i].name));
                json_object_set_new(jobj, "year", json_integer(me[i].year));
                json_object_set_new(jobj, "id", json_integer(me[i].id));
                json_array_append(ar, jobj);
                json_decref(jobj);
                j++;
            }
        }
    }
    if(j == 0){
        json_t * jobj = json_object();
        json_object_set_new(jobj, "Error", json_string("404 Not Found"));
        json_array_append(ar, jobj);
        json_decref(jobj);
    }
    const char * strDump = json_dumps(ar, JSON_COMPACT);
	NetMessage_setDataString(message, strDump);
	free((void *)strDump);
	json_decref(ar);
}

void reqFile(NetMessage * message){
    FILE * textFile = fopen("../data/data.txt", "r");
	if (textFile == NULL) {
		return ;
	}
	fseek(textFile, 0, SEEK_END); 
	long size = ftell(textFile);
	fseek(textFile, 0, SEEK_SET); 
	char * fcontent = malloc(size);
	fread(fcontent, 1, size, textFile);
    puts(fcontent);
	fclose(textFile);

    json_t * jobj = json_object();
    json_object_set_new(jobj, "title", json_string("data.txt"));
    json_object_set_new(jobj, "size", json_real(size));
    json_object_set_new(jobj, "data", json_string(fcontent));
    const char * strDump = json_dumps(jobj, JSON_COMPACT);
	NetMessage_setDataString(message, strDump);
	free((void *)strDump);
	json_decref(jobj);
    free(fcontent);
}

void reqFileData(NetMessage * message){
    FILE * textFile = fopen("../data/data.txt", "r");
	if (textFile == NULL) {
		return ;
	}
	fseek(textFile, 0, SEEK_END); 
	long size = ftell(textFile);
	fseek(textFile, 0, SEEK_SET); 
	char * fcontent = malloc(size);
	fread(fcontent, 1, size, textFile);
	fclose(textFile);
    char * str = fcontent;
	const char s[2] = " ";
	char * token = strtok(str, s);
    int i = 0;
    json_t * ar = json_array();
	while(token != NULL){
        i++;
        if(i%2 !=0){
            json_t * jobj = json_object();
            json_object_set_new(jobj,"data",json_string(token));
            json_array_append(ar, jobj);
            json_decref(jobj);
        }
        token = strtok(NULL, s);
    }
    const char * strDump = json_dumps(ar, JSON_COMPACT);
	NetMessage_setDataString(message, strDump);
	free((void *)strDump);
	json_decref(ar);
    free(fcontent);
}

void reqError(NetMessage * message){
    json_t * jobj = json_object();
    json_object_set_new(jobj, "Error", json_string("404 Not Found"));
    const char * strDump = json_dumps(jobj, JSON_COMPACT);
	NetMessage_setDataString(message, strDump);
	free((void *)strDump);
	json_decref(jobj);
}