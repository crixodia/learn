import { Injectable } from '@angular/core';
import { environment } from '../../../environments/environment';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Region } from '../../interfaces/region';

@Injectable({
  providedIn: 'root'
})
export class RegionService {
  private baseUrl = environment.BASE_URL;

  constructor(private http: HttpClient) { }

  getRegiones(): Observable<Region[]> {
    return this.http.get<Region[]>(`${this.baseUrl}/regiones`, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }
}
