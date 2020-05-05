# KYGUI

KY GUI is a socket client business application for KStyleYo, providing them with oversight and control of their digital operations

## Features

- Interface for user-configurable, delegated and parametrized process execution
- Compatibility with the KIQ messaging protocol
- Observation of KStyleYo's digital business operations

## Installation / Deployment

__Note:__ You must be running a Linux operating system with an X11 based Desktop Environment. Your X Server must be permitted to accept connections from localhost.

Docker is the preferred method of running KY GUI.
1. Navigate to https://hub.docker.com/repository/registry-1.docker.io/logicp/ky_gui/tags?page=1 to see a list of images
2. From the command line, login to docker hub and pull the production image
  - `docker pull logicp/ky_gui:production`
3. Run the container while linking the X11 socket from your X Server
  - `docker run --device /dev/dri/ -it --network host -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY logicp/ky_gui:production ./ky_gui <KServer IP> <Port> <Optional Argument>`
