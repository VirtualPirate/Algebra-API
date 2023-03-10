// Spawning socket server
import { spawn } from "child_process";
import net from "net";

import express from "express";
import cors from "cors";

import EventEmitter from "events";

let child;
let client;

// ? This program starts the Algebra Engine program which opens a socket connection.
function start() {
  console.log("Starting process...");
  child = spawn("./build/program");

  // ? Connects the Node Js server to the Algebra Engine through sockets
  client = net.createConnection(
    {
      port: 8080,
      host: "0.0.0.0",
    },
    () => {
      console.log("Connected to server");
    }
  );

  // ? And if the Algebra program stops running for any reason, it is restarted
  child.on("exit", (code, signal) => {
    console.log(`Process exited with code ${code} and signal ${signal}.`);
    console.log("Restarting process...");
    start();
  });
}

start(); // Starts the Algebra Engine

client.on("end", () => {
  console.log("Disconnected from server");
});

const emitter = new EventEmitter();

client.on("data", (data) => {
  const data_ = data.toString();
  // console.log(`[Received from cpp client]: ${data_}`);
  // Send the data received from the socket server back to the client via the API
  emitter.emit("server_recv", data_);
});

// API
const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());
app.use(
  cors({
    origin: "*",
  })
);

app.post("/", (req, res) => {
  // console.log(req.body);

  if (req.body.expression) {
    client.write(JSON.stringify(req.body));
  } else {
    return res.status(400).send({
      message: "The JSON body must contain expression as key field",
    });
  }

  emitter.on("server_recv", (data) => {
    console.log(`[Received from cpp client]: ${data}`);
    res.send(data);
    emitter.removeAllListeners();
  });
});

app.listen(PORT, () => {
  console.log(`Listening on Port 3000 ...`);
});
