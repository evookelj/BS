int add_user(char* user, char* pass) {
  char* query = "INSERT INTO users VALUES (\"";
  printf("A");
  query = strcat(query, user);
    printf("B");
  query = strcat(query, ", \"");
    printf("C");
  query = strcat(query, pass);
    printf("D");
  query = strcat(query, "\");");
  printf("%s\n", query);
}

int main() {
  add_user("grace", "katara");
  return 0;
}
