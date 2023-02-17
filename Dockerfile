FROM ubuntu:20.04

# Update the package repository and install necessary packages
RUN apt-get update && apt-get install -y make cmake clang npm

ENV NODE_VERSION=18.2.0
RUN curl https://raw.githubusercontent.com/nvm-sh/nvm/v0.38.0/install.sh | bash
ENV NVM_DIR=/root/.nvm
RUN . "$NVM_DIR/nvm.sh" && nvm install ${NODE_VERSION}
RUN . "$NVM_DIR/nvm.sh" && nvm use v${NODE_VERSION}
RUN . "$NVM_DIR/nvm.sh" && nvm alias default v${NODE_VERSION}
ENV PATH="/root/.nvm/versions/node/v${NODE_VERSION}/bin/:${PATH}"

# Set the working directory
WORKDIR /app

# Copy the source code to the container
COPY . .

RUN npm install
RUN cd build && cmake -DCMAKE_CXX_COMPILER=clang++ -G "Unix Makefiles" ../MathSolver
RUN cd build && make

EXPOSE 3000
CMD ["npm", "run", "start"]