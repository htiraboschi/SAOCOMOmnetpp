from skyfield.api import EarthSatellite, Topos, load
from datetime import datetime, timedelta

# Datos del TLE SAOCOM
tle_line1 = "1 43641U 18076A   19053.53309051 +.00000000 +00000+0 +14560-4 6 10788"
tle_line2 = "2 43641  97.8924 241.5083 0001459  89.8694 228.9627 14.82153246 20368"

# Coordenadas de la estación terrena Córdoba
ground_station = Topos(latitude_degrees=-31.522760365816634, longitude_degrees=-64.46468247944169, elevation_m=520)

# Inicializar el objeto Orbital
satellite = EarthSatellite(tle_line1, tle_line2, "SATELLITE_NAME", load.timescale())

# Rango de tiempo para calcular las visibilidades
ts = load.timescale()
start_time = datetime.strptime("2019/02/22 13:00:00", "%Y/%m/%d %H:%M:%S")
end_time = start_time + timedelta(days=7)  # 7 días de visibilidades

current_time = start_time
visibility_list = []
at_visibility = False

while current_time <= end_time:
    t = ts.utc(current_time.year, current_time.month, current_time.day,
               current_time.hour, current_time.minute, current_time.second)
    
    # Obtener la posición del satélite en las coordenadas de la estación terrena
    difference = satellite - ground_station
    topocentric = difference.at(t)
    alt, az, distance = topocentric.altaz()
    
    # Guardar la visibilidad si el satélite está por encima del horizonte
    if alt.degrees > 0:
        if not at_visibility:
            at_visibility = True
            visibility_starttime = current_time
            visibility_max_elevation = 0
        visibility_max_elevation = max(alt.degrees,visibility_max_elevation)
    else:
        if at_visibility:
            at_visibility = False
            visibility_list.append(("ETC",visibility_starttime,current_time, visibility_max_elevation))
    
    # Avanzar en el tiempo (por ejemplo, en intervalos de 10 segundos)
    current_time += timedelta(seconds=10)

# Imprimir el listado de visibilidades
event_file = open("events.txt","w")
event_file.write("station start_time end_time visibility_max_elevation \n")
for visibility in visibility_list:
    station, start_time, end_time, visibility_max_elevation = visibility
    event_file.write(f"{station} {start_time} {end_time} {visibility_max_elevation}\n")
event_file.close()
