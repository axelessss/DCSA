package_name=$1

rm -rf deb_package_${package_name}

mkdir -p deb_package_${package_name}
mkdir -p deb_package_${package_name}/bin
mkdir -p deb_package_${package_name}/DEBIAN
touch deb_package_${package_name}/DEBIAN/control

cat << EOF > deb_package_${package_name}/DEBIAN/control
Package: http-${package_name}-dsca
Version: 1.0
Architecture: amd64
Maintainer: NEgor + Sereja = <3
Description: http ${package_name} for DCSA labs
EOF

cp -r ${package_name}/build deb_package_${package_name}/bin/

dpkg-deb --build deb_package_${package_name}/ http-${package_name}-dsca.deb