#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address{
  int id;
  int set;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

struct Database {
  struct Address rows[MAX_ROWS];
};

struct Connection {
  FILE *file;
  struct Database *db;
};

void die(const char *message) {

  if(errno) {
    perror(message);
  }
  else {
    printf("Error: %s\n", message);
  }
  exit(1);
}

void Address_print(struct Address *addr) {
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
  int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
  if(rc != 1) die("Failed to load database.");
}

struct Connection *Database_open(const char *filename, char mode) {
  
  struct Connection *conn = malloc(sizeof(struct Connection));
  if(!conn) die("Memory error");

  conn->db = malloc(sizeof(struct Database));
  if(!conn->db) die("Memory error");

  if(mode == 'c') {
    conn->file = fopen(filename, "w");
  }
  else{
    conn->file = fopen(filename, "r+");

    if(conn->file) {
      Database_load(conn);
    }
  }
  if(!conn->file) die("Failed to open the file");

  return conn;
}

void Database_close(struct Connection *conn) {
  if(conn) {
    if(conn->file) fclose(conn->file);
    if(conn->db) free(conn->db);
    free(conn);
  }
}

void Database_write(struct Connection *conn) {
  rewind(conn->file);

  int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
  if(rc != 1) die("failed to write to database");

  rc = fflush(conn->file);
  if(rc == -1) die("Cannot flush database");
}

void Database_create(struct Connection *conn) {
  int i = 0;

  for(i = 0; i < MAX_ROWS; i++) {
    struct Address addr = {.id = 1, .set = 0};
    conn->db->rows[i] = addr; 
  }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
  struct Address *addr = &conn->db->rows[id];
  if(addr->set) die("Already set, delete it first");

  addr->set = 1;
  addr->id = id;
  char *res = strncpy(addr->name, name, MAX_DATA);
  if(!res) die("Name copy failed");

  res = strncpy(addr->email, email, MAX_DATA);
  if(!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
  struct Address *addr = &conn->db->rows[id];

  if(addr->set) {
    Address_print(addr);
  }
  else {
    die("Id not set");
  }
}

void Database_delete(struct Connection *conn, int id) {
  struct Address addr = {.id = 1, .set = 0};
  conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
  int i = 0;
  struct Database *db = conn->db;

  for(i = 0; i < MAX_ROWS; i++) {
    struct Address *cur = &db->rows[i];

    if(cur->set) {
      Address_print(cur);
    }
  }
}

void Database_find(struct Connection *conn, char *type, char *param) {
  struct Database *data = conn->db;
  int i = 0;

  if(strcmp(type, "name") == 0) {
    for(i = 0; i < MAX_ROWS; i++) {
      if(strcmp(data->rows[i].name, param) == 0) {
        Address_print(&data->rows[i]);
      }
    }
  }
  else if(strcmp(type, "email") == 0) {
    for(i = 0; i < MAX_ROWS; i++) {
      if(strcmp(data->rows[i].email, param) == 0) {
        Address_print(&data->rows[i]);
      }
    }  
  }
  else{
    die("You can only search by name or email");
  }
}

int main(int argc, char *argv[]) {
  if(argc < 3) die("USAGE: eleventh  <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];

  struct Connection *conn = Database_open(filename, action);
  int id = 0;

  if(argc > 3) id = atoi(argv[3]);
  if(id >= MAX_ROWS) die("There's not that many data");

  switch(action) {
    case 'c':
      Database_create(conn);
      Database_write(conn);
      break;

    case 'g':
      if(argc != 4) die("Need an id to get");

      Database_get(conn, id);
      break;

    case 's':
      if(argc != 6) die("Need id, name, email to set");

      Database_set(conn, id, argv[4], argv[5]);
      Database_write(conn);
      break;

    case 'd':
      if(argc != 4) die("Need id to delete");
      
      Database_delete(conn, id);
      Database_write(conn);
      break;

    case 'l':
      Database_list(conn);
      break;

    case 'f':
      if(argc != 5) die("Need the type and string to find");
      Database_find(conn, argv[3], argv[4]);
      break;

    default:
      die("Invalid action, only: c=create, g=get, s=set, d=delete, l=list");
  }
  Database_close(conn);

  return 0;
}




