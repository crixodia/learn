# Requirements
from fastapi import Depends, FastAPI, Body, HTTPException, Path, Query, Request
from fastapi.responses import HTMLResponse, JSONResponse
from pydantic import BaseModel, Field
from typing import Optional, List
from jwt_manager import create_token, validate_token
from fastapi.security import HTTPBearer

# App initialization
app = FastAPI(title="Mi API",
              description="Esta es una API de prueba",
              version="0.0.1")


# Models definition (Pydantic)
# Used to validate the data received in the request
# and to generate the documentation automatically in Schemas
class Movie(BaseModel):
    id: Optional[int] = None  #id: int | None = None  Optional parameter
    title: str = Field(
        default="My new movie default",  # Default value
        min_length=3,  # Validates the minimum length of the string
        max_length=100)  # Validates the maximum length of the string

    year: int = Field(
        ...,  # Required parameter
        gt=1900,  # Validates the value of the integer.
        lt=2023)  # Validates the value of the integer.

    class Config:
        # Replaces default values used before
        schema_extra = {  # Used to add extra information to the documentation
            "example": {
                "id": 1,
                "title": "My new movie config",
                "year": 2021
            }
        }


class User(BaseModel):
    password: str
    email: str


class JWTBearer(HTTPBearer):

    async def __call__(self, request: Request):
        auth = await super().__call__(request)
        data = validate_token(auth.credentials)
        if data['email'] != 'admin@gmail.com':
            raise HTTPException(status_code=403, detail="Invalid credentials")


@app.get('/', tags=['root'])  # tags are used to group endpoints
def message():
    # Returning a HTML response
    return HTMLResponse(content="<h1>API de prueba</h1>", status_code=200)


movies = [{
    "id": 1,
    "title": "The Shawshank Redemption",
    "year": 1994
}, {
    "id": 2,
    "title": "The Godfather",
    "year": 1972
}, {
    "id": 2,
    "title": "The Godfather: Part II",
    "year": 1974
}]


@app.get('/movies',
         tags=['movies'],
         response_model=List[Movie],
         dependencies=[Depends(JWTBearer())])
def get_movies() -> List[Movie]:
    # Returning a JSON response
    return JSONResponse(content=movies, status_code=200)


@app.get('/movies/{id}', tags=['movies'],
         response_model=List[Movie])  # Path parameters
def get_movie(id: int = Path(
    ge=1, le=2000, description="The ID of the movie to get")) -> List[Movie]:
    find = []
    for movie in movies:
        if movie['id'] == id:
            find.append(movie)
    return JSONResponse(content=find, status_code=200)


@app.get('/movies/', tags=['movies'])  # Query parameters
def get_movies(year: int = Query(
    default=1, le=2000,
    gt=1)):  # Default value for the parameter and Query validation
    find = []
    if year == -1:
        return movies

    for movie in movies:
        if movie['year'] == year:
            find.append(movie)
    return JSONResponse(content=find, status_code=200)


@app.post('/movies', tags=['movies'])  # Body parameters
def add_movie(movie: Movie):
    movies.append(movie)
    return JSONResponse(content={"message": "Movie added"}, status_code=201)


@app.put('/movies/{id}', tags=['movies'])  # Path and body parameters
def update_movie(id: int, movie: Movie):
    for m in movies:
        if m['id'] == id:
            m['title'] = movie.title
            m['year'] = movie.year
    return JSONResponse(content={"message": "Movie updated"}, status_code=201)


@app.delete('/movies/{id}', tags=['movies'])  # Path parameters
def delete_movie(id: int):
    for movie in movies:
        if movie['id'] == id:
            movies.remove(movie)
    return JSONResponse(content={"message": "Movie deleted"}, status_code=200)


@app.post('/login', tags=['auth'])
def login(user: User):
    if user.password == '1234' and user.email == 'admin@gmail.com':
        token: str = create_token({'email': user.email})
        return JSONResponse(content={"token": token}, status_code=200)