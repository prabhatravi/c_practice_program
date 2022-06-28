#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

int main(void)
{
    CURL *rdsp;
    struct curl_slist *rdscl=NULL;
    const char *perr = NULL;
    char *jsonstr = "{\"mac\": \"46:15:b9:64:f0:85\",\"dhcp\": {\"params\":\"1,3,6,15,26,28,51,58,59,43,114,108\", \"vendor\":\"android-dhcp-11\", \"hostname\":\"OnePlus-6T\"}}";

    rdscl = curl_slist_append(rdscl, "Content-Type: application/json");
    if (rdscl == NULL)
        return -1;
    rdscl = curl_slist_append(rdscl, "x-api-key: videvg12lk456665ffghsdghfgdrtadh8515");
    if (rdscl == NULL)
        return -1;

    rdsp = curl_easy_init();
    if (rdsp == NULL) {
        curl_slist_free_all(rdscl);
        return -1;
    }
    struct string s;
    init_string(&s);

    curl_easy_setopt(rdsp, CURLOPT_URL, "https://abc");
    curl_easy_setopt(rdsp, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(rdsp, CURLOPT_POSTFIELDS, jsonstr);
    curl_easy_setopt(rdsp, CURLOPT_HTTPHEADER, rdscl);
    curl_easy_setopt(rdsp, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(rdsp, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(rdsp, CURLOPT_WRITEDATA, &s);

    curl_easy_perform(rdsp);
    printf("resp: %s\n", s.ptr);
    free(s.ptr);

    curl_easy_cleanup(rdsp);
    curl_slist_free_all(rdscl);
}