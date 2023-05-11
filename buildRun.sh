cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja &&
ninja -C build && ./nosound $1 $2 $3 $4
