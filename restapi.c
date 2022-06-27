#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

int main(void)
{
  FILE *fp;
  char *lineptr;
  size_t n;
  char *jid;
  int i;

  CURL *curl;

  /* Make sure you set this appropriately*/
  char url[80];
  CURLcode res;

  fp = fopen("jobid.jid","w");

  strcpy(url,"https://api.picloud.com/r/3817/square_func/");

  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* Specify the user/pass */
    curl_easy_setopt(curl,CURLOPT_USERPWD,"pass:key");
    
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "x=5");

    /* For HTTPS */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    /* Ask curl to write the returned result to the file opened by fp*/
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    printf("\nResult of Operation:: %d\n", res);

    fflush(fp);
    fclose(fp);

    /* Back to standard output for the result*/
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);

    if(res==0)
      {
	fp = fopen("jobid.jid","r+");
	lineptr = NULL;
	n = 0;
	if(getline(&lineptr,&n,fp)!=-1)
	  {
	    jid = strtok(lineptr," ");
	    jid = strtok(NULL,"}");

	    printf("%s\n",jid);

	    /* delay loop */
	    /* TODO: better is to retrieve the status information from PiCloud itself
	       but that needs more coding :-/ */
	    sleep(20);

	    strcpy(url,"https://api.picloud.com/job/result/?jid=");
	    strcat(url,jid);

	    curl_easy_setopt(curl, CURLOPT_URL, url);
	    res = curl_easy_perform(curl);
	    printf("\nResult of Operation:: %d\n", res);
	  }

	    
	/* always cleanup */
	curl_easy_cleanup(curl);
      }
  }
  return 0;
}