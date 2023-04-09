from models.movie import MovieModel


class MovieService():
    def __init__(self, db) -> None:
        self.db = db

    def get_movies(self):
        movies = self.db.query(MovieModel).all()
        return movies

    def get_movie(self, id):
        movie = self.db.query(MovieModel).filter(MovieModel.id == id).first()
        return movie

    def get_movies_by_year(self, year):
        movies = self.db.query(MovieModel).filter(
            MovieModel.year == year).all()
        return movies

    def add_movie(self, movie):
        new_movie = MovieModel(**movie.dict())
        self.db.add(new_movie)
        self.db.commit()
        return new_movie

    def update_movie(self, id, movie):
        movie_to_change = self.db.query(
            MovieModel).filter(MovieModel.id == id).first()
        movie_to_change.title = movie.title
        movie_to_change.year = movie.year
        self.db.commit()
        return movie_to_change

    def delete_movie(self, id):
        movie = self.db.query(MovieModel).filter(MovieModel.id == id).first()
        self.db.delete(movie)
        self.db.commit()
        return movie
