from typing import List
from fastapi import APIRouter, Depends
from config.database import Session
from middlewares.jwt_bearer import JWTBearer
from fastapi.responses import HTMLResponse, JSONResponse
from fastapi.encoders import jsonable_encoder
from fastapi import Depends, HTTPException, Path, Query
from schemas.movie import Movie
from services.movie import MovieService

movie_router = APIRouter()


@movie_router.get('/', tags=['root'])  # tags are used to group endpoints
def message():
    # Returning a HTML response
    return HTMLResponse(content="<h1>API de prueba</h1>", status_code=200)


@movie_router.get('/movies',
                  tags=['movies'],
                  response_model=List[Movie],
                  dependencies=[Depends(JWTBearer())])
def get_movies() -> List[Movie]:
    movies = MovieService(Session()).get_movies()
    return JSONResponse(content=movies, status_code=200)


@movie_router.get('/movies/{id}', tags=['movies'],
                  response_model=List[Movie])  # Path parameters
def get_movie(id: int = Path(
        ge=1, le=2000, description="The ID of the movie to get")) -> List[Movie]:
    movie = MovieService(Session()).get_movie(id)
    if not movie:
        raise HTTPException(status_code=404, detail="Movie not found")
    return JSONResponse(content=jsonable_encoder(movie), status_code=200)


@movie_router.get('/movies/', tags=['movies'])  # Query parameters
def get_movies(year: int = Query(
        default=1, le=2000,
        gt=1)):  # Default value for the parameter and Query validation
    movies = MovieService(Session()).get_movies_by_year(year)
    if not movies:
        raise HTTPException(status_code=404, detail="Movies not found")
    return JSONResponse(content=jsonable_encoder(movies), status_code=200)


@movie_router.post('/movies', tags=['movies'])  # Body parameters
def add_movie(movie: Movie):
    new_movie = MovieService(Session()).add_movie(movie)
    return JSONResponse(content={"message": "Movie added"}, status_code=201)


@movie_router.put('/movies/{id}', tags=['movies'])  # Path and body parameters
def update_movie(id: int, movie: Movie):
    movie_to_change = MovieService(Session()).update_movie(id, movie)
    if not movie_to_change:
        raise HTTPException(status_code=404, detail="Movie not found")
    return JSONResponse(content={"message": "Movie updated"}, status_code=201)


@movie_router.delete('/movies/{id}', tags=['movies'])  # Path parameters
def delete_movie(id: int):
    movie = MovieService(Session()).delete_movie(id)
    if not movie:
        raise HTTPException(status_code=404, detail="Movie not found")
    return JSONResponse(content={"message": "Movie deleted"}, status_code=200)
