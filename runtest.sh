./server &
./client
lsof -i:4567 | grep TCP | cut -c 9-14 | xargs kill -9
