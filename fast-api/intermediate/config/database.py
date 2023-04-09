import os
from sqlalchemy import create_engine
from sqlalchemy.orm.session import sessionmaker
from sqlalchemy.ext.declarative import declarative_base

sqlite_database_name = '../database.sqlite'
base_dir = os.path.dirname(
    os.path.realpath(__file__))  # Path to the current file

database_url = f"sqlite:///{os.path.join(base_dir, sqlite_database_name)}"

engine = create_engine(database_url, echo=True)

Session = sessionmaker(bind=engine)

Base = declarative_base()