#!/usr/bin/env bash


IMAGE_NAME="logicp/ky_gui:production"
CWD=$(cd $(dirname $0) && pwd)

function rebuild_docker_image() {
  if [[ ! -f Dockerfile ]]; then
    echo "No Dockerfile found in $CWD"
    exit 1
  fi

  docker build --build-arg REBUILD_KY_GUI=$(date +%s) -t $IMAGE_NAME .
  if [[ $? -eq 0 ]]; then
    docker push $IMAGE_NAME
    if [[ $? -eq 0 ]]; then
      echo "Image updated on docker hub!"
      return 0
    fi
    echo "Image updated but not pushed to docker hub"
    return 1
  fi
  echo "Image could not be updated"
  return 1
}


rebuild_docker_image

if [[ $? -eq 0 ]]; then
  exit 0
else
  exit 1
fi

