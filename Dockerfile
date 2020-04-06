FROM debian
LABEL maintainer=thomas.calmant@inria.fr

# Sane environment
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8 DEBIAN_FRONTEND=noninteractive

# Install g++, make, git and graphviz (queryinterface uses "dot")
RUN apt update && \
    apt install -y build-essential git graphviz cmake

# Copy gMark from the local repository
COPY . /opt/gmark
WORKDIR /opt/gmark

# Compile it
RUN mkdir build && \
    cd build &&\
    cmake .. && \
    cmake --build . && \
    # Move everything
    mv bin/gmark-* /usr/local/bin/ && \
    cd .. && \
    # Clean up
    rm -fr build
