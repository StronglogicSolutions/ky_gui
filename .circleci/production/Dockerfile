FROM jonathonf/manjaro:latest
RUN pacman -Syu flatbuffers base-devel qt5-base noto-fonts ffmpeg adobe-source-han-sans-kr-fonts --noconfirm
RUN ln -sf /usr/share/zoneinfo/America/Toronto /etc/localtime
ARG REBUILD_KY_GUI=false
WORKDIR ky_gui
RUN curl -o ky_gui http://artifact.stronglogicsolutions.com/download?name=ky_gui
RUN chmod +x ky_gui
RUN mkdir config && echo '{"defaultApp":"instagram","fileDirectory":"/data/kstyleyo/instagram", "schedulerMode": "true"}' >> config/config.json
RUN mkdir -p assets/previews
