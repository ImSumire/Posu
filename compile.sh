mkdir -p dist

gcc -o dist/main src/main.cpp -lraylib -lm -lstdc++
