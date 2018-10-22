FROM debian
LABEL maintainer=thomas.calmant@inria.fr

# Sane environment
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8 DEBIAN_FRONTEND=noninteractive

# Install g++, make, git and graphviz (queryinterface uses "dot")
RUN apt update && \
    apt install -y build-essential git graphviz

# Clone gMark
COPY . /opt/gmark
WORKDIR /opt/gmark

# Compile it
RUN cd demo/scripts && \
    ./compile-all.sh

# Move everything
RUN cp src/test /usr/local/bin/gmark-gen && \
    cp src/querytranslate/test /usr/local/bin/gmark-translate && \
    cp src/queryinterface/test /usr/local/bin/gmark-interface
