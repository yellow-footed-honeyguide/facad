build: check clean_build clean_out
	cd ./build && meson ..  >> ./.log && ninja >> ./.log
	cd ./build && mkdir -p ../out/bin/ && mv ./facad ../out/bin/

clean_out:
	rm -rf ./out && mkdir ./out

clean_build:
	rm -rf ./build && mkdir ./build 

clean_out:
	rm -rf ./out && mkdir ./out
	mkdir ./out/bin
	mkdir ./out/deb

build_dev:


build_deb: build check_deb
	mkdir -p ./out/deb/facad/usr/local/bin
	mkdir -p ./out/deb/facad/DEBIAN
	cp ./out/bin/facad ./out/deb/facad/usr/local/bin/
	echo "Package: facad" > ./out/deb/facad/DEBIAN/control
	echo "Version: 1.0" >> ./out/deb/facad/DEBIAN/control
	echo "Section: utils" >> ./out/deb/facad/DEBIAN/control
	echo "Priority: optional" >> ./out/deb/facad/DEBIAN/control
	echo "Architecture: amd64" >> ./out/deb/facad/DEBIAN/control
	echo "Maintainer: Sergey Veneckiy <s.venetsky@gmail.com>" >> ./out/deb/facad/DEBIAN/control
	echo "Description: A modern, colorful directory listing tool for the command line." >> ./out/deb/facad/DEBIAN/control
	dpkg-deb --build ./out/deb/facad
	rm -rf ./out/deb/facad

check:
	@command -v meson >/dev/null 2>&1 || { echo "meson is not installed. Please install meson and try again."; exit 1; }
	@command -v ninja >/dev/null 2>&1 || { echo "ninja is not installed. Please install ninja and try again."; exit 1; }

check_deb:
	@command -v dpkg-deb >/dev/null 2>&1 || { echo "dpkg-deb is not installed. Please install dpkg-deb and try again."; exit 1; }

install: build
	cd ./build && sudo ninja install
