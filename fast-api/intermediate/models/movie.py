from config.database import Base
from sqlalchemy import Column, Integer, String


class MovieModel(Base):
    __tablename__ = "movies"
    id = Column(Integer, primary_key=True)
    title = Column(String)
    year = Column(Integer)

# Nice to try: https://sqlmodel.tiangolo.com/
