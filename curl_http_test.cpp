#include <string>
#include <iostream>
#include <curl/curl.h>

static size_t write_callback(void *data, size_t elem_size, size_t elem_cnt, void *write_stream)
{
    size_t mem_readed = elem_size * elem_cnt;

    std::string *str = static_cast<std::string*>(write_stream);

    str->append(static_cast<char*>(data), mem_readed);

    return mem_readed;
}

int main()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::string read_buffer;

    CURL *curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        CURLcode res = curl_easy_perform(curl);

        if (CURLE_OK != res)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            std::cout << read_buffer << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}

