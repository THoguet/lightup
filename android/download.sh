#!/bin/bash

cd jni

wget -c https://www.libsdl.org/release/SDL2-2.0.20.zip
unzip SDL2-2.0.20.zip
mv SDL2-2.0.20 SDL

wget -c https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.18.zip
unzip SDL2_ttf-2.0.18.zip
mv SDL2_ttf-2.0.18 SDL_ttf

wget -c https://libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.zip
unzip SDL2_mixer-2.0.4.zip
mv SDL2_mixer-2.0.4 SDL_mixer

wget -c https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.zip
unzip SDL2_image-2.0.5.zip
mv SDL2_image-2.0.5 SDL_image

cd ..
