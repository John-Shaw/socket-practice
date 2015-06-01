//
//  Dictionary.h
//  socket-server
//
//  Created by 邵建勇 on 15/6/1.
//  Copyright (c) 2015年 John Shaw. All rights reserved.
//

#ifndef socket_server_Dictionary_h
#define socket_server_Dictionary_h
typedef struct dict *Dict;

/* create a new empty dictionary */
Dict DictCreate(void);

/* destroy a dictionary */
void DictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
 void DictInsert(Dict, const char *key, const char *value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char *DictSearch(Dict, const char *key);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void DictDelete(Dict, const char *key);

#endif