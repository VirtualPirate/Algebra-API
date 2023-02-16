// Spawning socket server
import { spawn } from "child_process";
import net from "net";

import express from "express";
import cors from "cors";

import EventEmitter from "events";

let child;

function start() {
  console.log("Starting process...");
  child = spawn("./build/program");

  child.on("exit", (code, signal) => {
    console.log(`Process exited with code ${code} and signal ${signal}.`);
    console.log("Restarting process...");
    start();
  });
}

start();

// Creating Socket Connection
const client = net.createConnection(
  {
    port: 8080,
  },
  () => {
    console.log("Connected to server");
  }
);

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

app.get("/", (req, res) => {
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
