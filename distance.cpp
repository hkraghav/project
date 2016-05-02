#include <cmath>
#include <stdio.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0
#define LATITUDE 0
#define LONGITUDE 1
#define DATE 5
#define TIME 6
using namespace std;

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / pi);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distance(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  double result = 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
  return result*1000;
}

vector< string >  extract_data(string s)
{
    vector< string > data(7);
    int len = s.length();
    int i = 0 ;
    int index = 0 ;
    int x;
    while( i < len )
    {
        while(s[i] != ',' && i < len )
        {
            data[index] += s[i];
            i++;
        }
        i++;
        index++;
    }
    return data;
}

long long seconds( string date1 , string time1 , string date2 , string time2 )
{
    long long year, month , days , hour , minute , second ;
    year = atoi( (date2.substr(0,4)).c_str() )  - atoi( (date1.substr(0,4)).c_str() );
    month = atoi( (date2.substr(5,2)).c_str() )  - atoi( (date1.substr(5,2)).c_str() );
    days = atoi( (date2.substr(8,2)).c_str() )  - atoi( (date1.substr(8,2)).c_str() );
    hour = atoi( (time2.substr(0,2)).c_str() )  - atoi( (time1.substr(0,2)).c_str() )  ;
    minute = atoi( (time2.substr(3,2)).c_str() )  - atoi( (time1.substr(3,2)).c_str()  );
    second = atoi( (time2.substr(6,2)).c_str() )  - atoi( (time1.substr(6,2)).c_str()  );
    return year*365*24*3600 + month*30*24*3600 + days*24*3600 + hour*3600 + minute*60 + second;
}

int main()
{
    ifstream mystream("filename.txt");
    string line1 , line2 ;

    for(int i = 0 ; i < 7 ; i++)
    {
        getline(mystream,line1);
    }

    getline(mystream,line1);
    while( getline(mystream,line2)  )
    {
        vector< string > data1 = extract_data(line1);
        vector< string > data2 = extract_data(line2);
        double lat1 , lat2 , long1 , long2;
        lat1 = atof( data1[LATITUDE].c_str() );
        lat2 = atof( data2[LATITUDE].c_str() );
        long1 = atof( data1[LONGITUDE].c_str() );
        long2 = atof( data2[LONGITUDE].c_str() );
        double travelled = distance(lat1,long1,lat2,long2);
        long long duration = seconds(data1[DATE],data1[TIME],data2[DATE],data2[TIME]);
        double speed = travelled/duration;
        FILE *fp = fopen("speedlog.txt","a");
        fprintf(fp,"%f\t%f\t%f\t%lld\n",travelled,speed,speed*3.6,duration);
        line1 = line2 ;
    }
    return 0;
}
