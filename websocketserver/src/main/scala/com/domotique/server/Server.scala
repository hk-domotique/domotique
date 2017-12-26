package com.domotique.server

import akka.actor.ActorSystem
import akka.http.scaladsl.Http
import akka.http.scaladsl.model.ws.{Message, TextMessage}
import akka.stream.ActorMaterializer
import akka.stream.scaladsl.Flow

import scala.concurrent.Await
import scala.io.StdIn

import scala.concurrent.duration._

object Server extends App {

  implicit val actorSystem = ActorSystem("akka-system")
  implicit val flowMaterializer = ActorMaterializer()

  val interface = "0.0.0.0"
  val port = 8080

  import akka.http.scaladsl.server.Directives._

  val echoService: Flow[Message, Message, _] = Flow[Message].map {
    case TextMessage.Strict(txt) => {
      println("Arrived " + txt)
      TextMessage("ECHO: " + txt)
    }
    case TextMessage.Streamed(textStream) => {
      textStream.map(f => println(f))
      println("Arrived ss" + textStream)
      TextMessage("ECHO not good ")
    }
    case _ => {
      println("Message type unsupported")
      TextMessage("Message type unsupported")
    }
  }

  val route = get {
    pathEndOrSingleSlash {
      print("Arrived message from client")
      handleWebSocketMessages(echoService)
    }
  }

  val binding = Http().bindAndHandle(route, interface, port)
  println(s"Server is now online at http://$interface:$port\nPress RETURN to stop...")
  StdIn.readLine()

  import actorSystem.dispatcher

  binding.flatMap(_.unbind()).onComplete(_ => actorSystem.terminate())
  println("Server is down...")

}