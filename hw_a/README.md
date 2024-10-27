The task is to implement the function `std::list<Place> find_path(const Map &map)`, 
which finds the shortest possible route on the given map that collects at least one part of each type. 
The route must start at `map.start` and end at `map.end`, and each consecutive pair of locations must be connected by a corridor. 
If such a route does not exist, return an empty list. 

The Map structure includes the following attributes:

`places`: the number of rooms

`start`, `end`: indices of the rooms where Max starts and where he needs to deliver the collected items, 
respectively (indexed from zero)

`connections`: a list of room pairs describing the corridors between them

`items`: a list of lists where `items[i]` specifies the rooms containing the i-th item type

That room indices are always in the range 0 to places - 1 (inclusive).
