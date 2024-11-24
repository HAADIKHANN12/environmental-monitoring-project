#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "weather.h"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    const char *ignored_message = "Send some mail, I'll try my best";
    if (strstr((char *)contents, ignored_message) == NULL) {
        ((char *)userp)[size * nmemb] = '\0';
        strcat((char *)userp, contents);
    }
    return size * nmemb;
}

void save_raw_data(const char *filename, const char *response) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", response);
        fclose(file);
    } else {
        perror("Error opening raw data file");
    }
}

int get_weather_data(const char *url, char *response) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Error initializing curl\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Curl error: %s\n", curl_easy_strerror(res));
        return 1;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    printf("Received Data: %s\n", response);
    return 0;
}

int parse_json(const char *json_data, EnvironmentalData *data) {
    struct json_object *parsed_json;
    struct json_object *location;
    struct json_object *current;
    struct json_object *temp;
    struct json_object *humid;
    struct json_object *city;
    struct json_object *country;

    parsed_json = json_tokener_parse(json_data);
    if (parsed_json == NULL) {
        fprintf(stderr, "Error parsing JSON\n");
        return 1;
    }

    location = json_object_object_get(parsed_json, "location");
    current = json_object_object_get(parsed_json, "current");

    city = json_object_object_get(location, "name");
    country = json_object_object_get(location, "country");
    temp = json_object_object_get(current, "temperature");
    humid = json_object_object_get(current, "humidity");

    strcpy(data->city, json_object_get_string(city));
    strcpy(data->country, json_object_get_string(country));
    data->temperature = json_object_get_double(temp);
    data->humidity = json_object_get_int(humid);

    printf("City: %s\n", data->city);
    printf("Country: %s\n", data->country);
    printf("Temperature: %.2f°C\n", data->temperature);
    printf("Humidity: %d%%\n", data->humidity);

    return 0;
}

void write_to_file(const char *filename, const EnvironmentalData *data) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "City: %s, Country: %s, Temperature: %.2f°C, Humidity: %d%%\n",
                data->city, data->country, data->temperature, data->humidity);
        fclose(file);
    } else {
        perror("Error opening file");
    }
}

int check_alerts(const EnvironmentalData *data) {
    int alert_sent = 0;

    if (data->temperature > 30.0) {
        printf("Alert: High temperature in %s!\n", data->city);
        system("notify-send 'High Temperature Alert' 'Temperature exceeds 30°C!'");
        alert_sent = 1;
    }

    if (data->humidity < 20) {
        printf("Alert: Low humidity in %s!\n", data->city);
        system("notify-send 'Low Humidity Alert' 'Humidity below 20%'");
        alert_sent = 1;
    }

    return alert_sent;
}

int main() {
    char *response = (char *)malloc(2048 * sizeof(char));
    if (response == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    EnvironmentalData data;

    const char *url = "http://api.weatherstack.com/current?access_key=b5d69807151e6a5ccce627d3305c4420&query=Gaborone";

    if (get_weather_data(url, response) == 0) {
        save_raw_data("raw_data.txt", response);
        if (parse_json(response, &data) == 0) {
            write_to_file("environmental_data.txt", &data);
            check_alerts(&data);
        } else {
            fprintf(stderr, "Error parsing JSON data\n");
        }
    } else {
        fprintf(stderr, "Error fetching data from API\n");
    }

    free(response);
    printf("Press any key to exit...\n");
    getchar();
    return 0;
}
