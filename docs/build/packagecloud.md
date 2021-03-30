# Installing packagecloud for pushing binaries.

```
gem install rake &&
gem install package_cloud
```

# Uploading build artificats to packagecloud.

```
package_cloud push nedrysoft/pingnoo/ubuntu/bionic *ubuntu18.04*.deb
package_cloud push nedrysoft/pingnoo/ubuntu/focal *ubuntu20.04*.deb
package_cloud push nedrysoft/pingnoo/ubuntu/xenial *ubuntu20.10*.deb

package_cloud push nedrysoft/pingnoo/debian/buster *debian10*.deb

package_cloud push nedrysoft/pingnoo/fedora/32 *fc32*.rpm
package_cloud push nedrysoft/pingnoo/fedora/33 *fc33*.rpm
```