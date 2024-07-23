# Maintainer: Astro
pkgname='cosmic-ascent'
pkgver=1
pkgrel=1
pkgdesc="A 2D top-down rogue-like currently in development"
arch=('x86_64')
url="https://github.com/Astroncity/Cosmic-Ascent"
license=('MIT')
depends=('glibc')
makedepends=('git')
source=("$pkgname::git+https://github.com/Astroncity/Cosmic-Ascent.git")
md5sums=('SKIP')

pkgver(){
	cd "$pkgname"
	git describe --long --tags --abbrev=7 | sed 's/\([^-]*-g\)/r\1/;s/-/./g'
}

build() {
	cd "$pkgname"
	make
}

package() {
	cd "$pkgname"
	install -Dm755 ./Cosmic-Ascent "$pkgdir/usr/bin/Cosmic-Ascent"
	install -Dm644 ./README.md "$pkgdir/usr/share/doc/Cosmic-Ascent/README.md"
}
