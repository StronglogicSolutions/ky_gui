FROM vookimedlo/ubuntu-qt:latestDistroOfficial_gcc_bionic
RUN apt-get update && apt-get install -y software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN add-apt-repository ppa:team-xbmc/ppa
RUN apt-get update && apt-get install -y qttools5-dev gcc-9 g++-9 flatbuffers flatbuffers-dev
# RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9
