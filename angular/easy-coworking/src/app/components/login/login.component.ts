import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormBuilder, Validators, ReactiveFormsModule, FormGroup } from '@angular/forms';
import { Router, RouterModule } from '@angular/router';
import { AuthService } from '../../services/auth/auth.service';
import { AuthInput } from '../../interfaces/auth';
import { HttpClientModule } from '@angular/common/http';
import { UsuarioService } from '../../services/usuario/usuario.service';
import { EMPTY, catchError, switchMap } from 'rxjs';
import { ToastService } from '../../services/toast/toast.service';
import { TOAST_TYPE } from '../../shared/components/toast/toast.component';


@Component({
  selector: 'app-login',
  standalone: true,
  providers: [AuthService, UsuarioService],
  imports: [CommonModule, ReactiveFormsModule, RouterModule, HttpClientModule],
  templateUrl: './login.component.html',
  styleUrl: './login.component.css'
})
export class LoginComponent implements OnInit {
  loginForm: FormGroup;

  constructor(private fb: FormBuilder, private authService: AuthService, private usuarioService: UsuarioService, private router: Router, private toastService: ToastService) {
    this.loginForm = this.fb.group({
      email: ['', [Validators.required, Validators.email]],
      contrasena: ['', [Validators.required]]
    });
  }

  login() {
    const postData = { ...this.loginForm.value };

    this.authService.login(postData as AuthInput).pipe(
      switchMap(data => {
        sessionStorage.setItem('access_token', data.access_token);
        return this.usuarioService.getUsuarioByEmail(postData.email);
      }),
      catchError(error => {
        if (error.status === 401) {
          this.toastService.showToast('Error al iniciar sesión', 'Usuario o contraseña incorrectos', TOAST_TYPE.error);
        } else {
          this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
        }
        return EMPTY; // para indicar un flujo vacío en caso de error.
      })
    ).subscribe(usuario => {
      sessionStorage.setItem('usuario', JSON.stringify(usuario));
      this.router.navigate(['/home']);
      (error: any) => {
        this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
      }
    });
  }

  ngOnInit(): void {
  }

  get email() { return this.loginForm.get("email"); }
  get contrasena() { return this.loginForm.get("contrasena"); }
}
