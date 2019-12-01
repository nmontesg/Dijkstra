# -*- coding: utf-8 -*-

#%% Load data on flights
import pandas as pd
import numpy as np
data = pd.read_csv("flights.csv")
data.apply (pd.to_numeric, errors='coerce')
data.dropna(inplace=True)
data[['dep_time', 'arr_time']] = data[['dep_time', 'arr_time']].astype(int)
flights_raw = data[['year', 'month', 'day', 'dep_time', 'arr_time', 
                'origin', 'dest']]
flights_raw['dep_hour'] = flights_raw['dep_time']//100
flights_raw['dep_min'] = flights_raw['dep_time']%100
flights_raw['arr_hour'] = flights_raw['arr_time']//100
flights_raw['arr_min'] = flights_raw['arr_time']%100
flights_raw['dep_date'] = pd.to_datetime(dict(year=flights_raw.year,
       month=flights_raw.month,
       day=flights_raw.day,
       hour=flights_raw.dep_hour,
       minute=flights_raw.dep_min))
flights_raw['arr_date'] = pd.to_datetime(dict(year=flights_raw.year,
       month=flights_raw.month,
       day=flights_raw.day,
       hour=flights_raw.arr_hour,
       minute=flights_raw.arr_min))
flights = flights_raw[['origin', 'dest', 'dep_date', 'arr_date']]
flights.loc[flights.arr_date<flights.dep_date, 'arr_date'] += np.timedelta64(1,'D')
#%%

#%% Extract a list of all possible airports
airports = pd.concat([flights['origin'], flights['dest']]).to_frame(name=
                    'code')
airports.drop_duplicates(keep='first', inplace=True)
airports.reset_index(inplace=True, drop=True)
# for now, any airport has been visited
airports['visited'] = False
# for now, any airport has any predecessor
airports['predecessor'] = ""
# set cost of all airports to infinity
airports['cost'] = np.datetime64(pd.Timestamp.max)
# index of flight that took us to that airport: unknown for the moment
airports['flight_id'] = np.nan
flights.origin = airports.code.sample(327346, replace=True).values
flights.dest = airports.code.sample(327346, replace=True).values
#%%

#%% Select source and destination
selection = airports.code.sample(2)
source = selection.values[0]
dest= selection.values[1]
# set cost of source to 0 (kind of)
airports.loc[airports.code==source, 'cost'] = np.datetime64(pd.Timestamp.min)
#%%

#%% DIJKSTRA ALGORITHM
current = ""
while (current != dest):
# among airports that have not been visited, extract that with lowest cost
    candidates = airports.loc[airports.visited==False]
    current = candidates.loc[candidates.cost.idxmin()]['code']
# set current time to arrival to current node
    now = np.datetime64(airports.loc[airports.code==current, 'cost'].iloc[0])
# set current airport to visited
    airports.loc[airports.code==current, 'visited'] = True
# extract outgoing flights from current airport
    flights_from_cur = flights.loc[(flights.origin==current)]
# extract possible successors
    successors = flights_from_cur.dest.drop_duplicates()
# scan successors
    for succ in successors:
        if (airports.loc[airports.code==succ]['visited'].values[0]==True):
            continue
    # flights from current to successor that have not departed yet
        poss_flights = flights_from_cur.loc[(flights_from_cur.dest==succ) & (flights_from_cur.dep_date>now)]
    # select the flight that arrives the earliest among the possible ones
        best_flight = poss_flights.loc[poss_flights.arr_date.idxmin()]
    # extract arrival time of best flight
        arr_time_to_succ = np.datetime64(best_flight.arr_date)
    # extract index of best flight
        best_flight_id = poss_flights.index[poss_flights.arr_date==arr_time_to_succ].tolist()[0]
    # update successor if necessary
        if ((arr_time_to_succ < airports.loc[airports.code==succ, 'cost']).values[0]):
            airports.loc[airports.code==succ, 'cost'] = arr_time_to_succ
            airports.loc[airports.code==succ, 'predecessor'] = current
            airports.loc[airports.code==succ, 'flight_id'] = best_flight_id
#%%
            
#%% Print route to file
backtrack = dest
f = open("%s-%s.txt" % (source, dest), "a")
f.write("ROUTE FROM %s TO %s\n" % (source, dest))
f.write("--------------\n")

while (backtrack != source):
    flight_id = int(airports.loc[airports.code==backtrack, 'flight_id'].values[0])
    flight = flights.ix[[flight_id]]
    flight.to_csv(f, header=False)
    f.write("--------------\n")
    pred = airports.loc[airports.code==backtrack, 'predecessor'].values[0]
    backtrack = pred
    
f.close()
#%%