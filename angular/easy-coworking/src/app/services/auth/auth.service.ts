import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { AuthInput, AuthOutput } from '../../interfaces/auth';
import { Observable } from 'rxjs';
import { environment } from '../../../environments/environment';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  private baseUrl = environment.BASE_URL;

  constructor(private http: HttpClient) { }

  login(authData: AuthInput): Observable<AuthOutput> {
    return this.http.post<AuthOutput>(`${this.baseUrl}/login`, authData);
  }

  requestResetPassword(email: string): Observable<any> {
    return this.http.get<any>(`${this.baseUrl}/request-reset-password/${email}`);
  }
}
