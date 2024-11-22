#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "weather.h"

// This callback handles writing the API response into a string buffer
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((char *)userp)[size * nmemb] = '\0';
    strcat((char *)userp, contents); // Append the new data
    return size * nmemb;
}

// Save raw JSON response to a file for debugging or record-keeping
void save_raw_data(const char *filename, const char *response) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", response); // Write the response to the file
        fclose(file);
    } else {
        perror("Error opening raw data file"); // Print an error message if the file can't be opened
    }
}

// Use cURL to fetch weather data from the API and store it in a buffer
int get_weather_data(const char *url, char *response) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT); // Initialize cURL globally
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Error initializing curl\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url); // Set the API URL
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Set the callback for response data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response); // Set where to store the response

    res = curl_easy_perform(curl); // Perform the API request
    if (res != CURLE_OK) {
        fprintf(stderr, "Curl error: %s\n", curl_easy_strerror(res)); // Log any cURL errors
        return 1;
    }

    curl_easy_cleanup(curl); // Clean up cURL resources
    curl_global_cleanup(); // Clean up cURL global settings

    printf("Received Data: %s\n", response); // Log the raw response
    return 0;
}

// Parse the JSON response and fill the EnvironmentalData struct
int parse_json(const char *json_data, EnvironmentalData *data) {
    struct json_object *parsed_json;
    struct json_object *location;
    struct json_object *current;
    struct json_object *temp;
    struct json_object *humid;
    struct json_object *city;
    struct json_object *country;

    parsed_json = json_tokener_parse(json_data); // Parse JSON string into JSON object
    if (parsed_json == NULL) {
        fprintf(stderr, "Error parsing JSON\n"); // Error if JSON is invalid
        return 1;
    }

    // Extract data from JSON object
    location = json_object_object_get(parsed_json, "location");
    current = json_object_object_get(parsed_json, "current");

    city = json_object_object_get(location, "name");
    country = json_object_object_get(location, "country");
    temp = json_object_object_get(current, "temperature");
    humid = json_object_object_get(current, "humidity");

    // Populate the EnvironmentalData struct
    strcpy(data->city, json_object_get_string(city));
    strcpy(data->country, json_object_get_string(country));
    data->temperature = json_object_get_double(temp);
    data->humidity = json_object_get_int(humid);

    // Log the parsed weather data
    printf("City: %s\n", data->city);
    printf("Country: %s\n", data->country);
    printf("Temperature: %.2f°C\n", data->temperature);
    printf("Humidity: %d%%\n", data->humidity);

    return 0;
}

// Save the parsed weather data to a file in a human-readable format
void write_to_file(const char *filename, const EnvironmentalData *data) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "City: %s, Country: %s, Temperature: %.2f°C, Humidity: %d%%\n",
                data->city, data->country, data->temperature, data->humidity); // Write weather data
        fclose(file);
    } else {
        perror("Error opening file"); // Print error if unable to open file
    }
}

// Check weather conditions and raise alerts if necessary
void check_alerts(const EnvironmentalData *data) {
    if (data->temperature > 30.0) { // Alert for high temperature
        printf("Alert: High temperature in %s!\n", data->city);
        system("notify-send 'High Temperature Alert' 'Temperature exceeds 30°C!'"); // Desktop notification
    }
    if (data->humidity < 20) { // Alert for low humidity
        printf("Alert: Low humidity in %s!\n", data->city);
        system("notify-send 'Low Humidity Alert' 'Humidity below 20%'"); // Desktop notification
    }
}

// The main function: entry point of the program
int main() {
    // Allocate memory for storing the API response
    char *response = (char *)malloc(2048 * sizeof(char));
    if (response == NULL) {
        fprintf(stderr, "Memory allocation failed\n"); // Error if memory allocation fails
        return 1;
    }

    EnvironmentalData data; // Declare an instance of EnvironmentalData

    // API URL for querying weather data
    const char *url = "http://api.weatherstack.com/current?access_key=b5d69807151e6a5ccce627d3305c4420&query=sydney";

    // Fetch, save, parse, and evaluate weather data
    if (get_weather_data(url, response) == 0) {
        save_raw_data("raw_data.txt", response); // Save the raw response
        if (parse_json(response, &data) == 0) {
            write_to_file("environmental_data.txt", &data); // Save parsed data
            check_alerts(&data); // Check for weather alerts
        } else {
            fprintf(stderr, "Error parsing JSON data\n");
        }
    } else {
        fprintf(stderr, "Error fetching data from API\n");
    }

    free(response); // Free allocated memory
    printf("Press any key to exit...\n");
    getchar(); // Wait for user input before exiting
    return 0; // Program ends here
}
