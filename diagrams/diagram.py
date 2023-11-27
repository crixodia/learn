from diagrams import Diagram

from diagrams.firebase.base import Firebase
from diagrams.firebase.develop import Firestore, Authentication, Hosting
from diagrams.gcp.compute import ComputeEngine
from diagrams.programming.framework import React
from diagrams.custom import Custom


with Diagram("Infraestructura", show=False):
    auth = Authentication("Authentication")
    hosting = Hosting("Hosting")
    firestore = Firestore("Firestore")
    react = React("React")
    firebase = Firebase()
    google = Custom("Google Cloud", "./google.png")
    paypal = Custom("Paypal", "./paypal.png")
    mailgun = Custom("Mailgun", "./email.png")

    auth >> firebase >> hosting
    firestore >> firebase >> hosting
    react >> hosting

    firebase >> google
    paypal >> google
    mailgun >> google