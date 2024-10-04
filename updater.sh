#!/bin/bash

function build {
    cd build
    cmake ..
    make
}

echo "LeoPlay updater 0.1"

echo "Checking update information..."
git fetch origin

BRANCH=${1:-$(git branch --show-current)}

LOCAL_COMMIT=$(git rev-parse "$BRANCH")

REMOTE_COMMIT=$(git rev-parse "origin/$BRANCH")

BASE_COMMIT=$(git merge-base "$BRANCH" "origin/$BRANCH")

if [ "$LOCAL_COMMIT" = "$REMOTE_COMMIT" ]; then
    echo "Everything up to date."
elif [ "$LOCAL_COMMIT" = "$BASE_COMMIT" ]; then
    echo "Update available at remote origin."
    echo "Downloading update..."
    git pull origin "$BRANCH"
    if [ $? -eq 0 ]; then
        echo "Update successful."
        #build (uncomment in release)
    else
        echo "Error while pulling."
        exit 1
    fi
    # update
elif [ "$REMOTE_COMMIT" = "$BASE_COMMIT" ]; then
    echo "Local branch ahead. Push required"
    echo "Pushing update to remote..."
    git push origin "$BRANCH"
    if [ $? -eq 0 ]; then
        echo "Successfully pushed update."
    else
        echo "Error while pushing."
        exit 2
    fi
    # push
else
    echo "Local and remote diverged. Merge required"
    exit 3
    # Problem.
fi