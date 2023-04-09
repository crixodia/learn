# Models definition (Pydantic)
# Used to validate the data received in the request
# and to generate the documentation automatically in Schemas
from typing import Optional
from pydantic import BaseModel, Field


class Movie(BaseModel):
    id: Optional[int] = None  # id: int | None = None  Optional parameter
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
