#ifndef WEATHER_H
#define WEATHER_H

typedef struct {
    double temperature;
    int humidity;
    char city[50];
    char country[50];
} EnvironmentalData;

int get_weather_data(const char *url, char *response);
int parse_json(const char *json_data, EnvironmentalData *data);
void write_to_file(const char *filename, const EnvironmentalData *data);
void check_alerts(const EnvironmentalData *data);

#endif // WEATHER_H
