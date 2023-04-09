# Requirements
from fastapi import FastAPI
from routers.movie import movie_router
from routers.user import user_router
from config.database import engine, Base
from middlewares.error_handler import ErrorHandler

# App initialization
app = FastAPI(
    title="Mi API",
    description="Esta es una API de prueba",
    version="0.0.1",
)

app.add_middleware(ErrorHandler)

app.include_router(movie_router)
app.include_router(user_router)

Base.metadata.create_all(bind=engine)
