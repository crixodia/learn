# Requirements
from fastapi import Depends, FastAPI, Body, HTTPException, Path, Query, Request
from fastapi.responses import HTMLResponse, JSONResponse

# App initialization
app = FastAPI(title="Mi API",
              description="Esta es una API de prueba",
              version="0.0.1")

# Hello world
@app.get('/', tags=['root'])  # tags are used to group endpoints
def message():
    # Returning a HTML response
    return HTMLResponse(content="<h1>API de prueba</h1>", status_code=200)