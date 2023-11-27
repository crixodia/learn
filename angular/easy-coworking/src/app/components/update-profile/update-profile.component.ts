import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormBuilder, FormGroup, ReactiveFormsModule, Validators } from '@angular/forms';
import { UsuarioService } from '../../services/usuario/usuario.service';
import { Router, RouterModule } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';
import { UsuarioRegister } from '../../interfaces/usuario';
import { name_pattern, passwordMatchValidator, password_pattern } from '../../shared/password-match.directive';

@Component({
  selector: 'app-complete-profile',
  standalone: true,
  providers: [UsuarioService],
  imports: [CommonModule, ReactiveFormsModule, HttpClientModule, RouterModule],
  templateUrl: './update-profile.component.html',
  styleUrl: './update-profile.component.css'
})
export class UpdateProfileComponent implements OnInit {
  completeProfileForm: FormGroup;

  constructor(private fb: FormBuilder, private router: Router, private usuarioService: UsuarioService) {
    this.completeProfileForm = this.fb.group({
      empresa: ['', [Validators.required]],
      nombre: ['', [Validators.required, Validators.pattern(name_pattern)]],
      direccion: ['', [Validators.required]],
      telefono: ['', [Validators.required]],
      cargo: [''],
      contrasena: ['', [Validators.required, Validators.pattern(password_pattern)]],
      confirm_contrasena: ['', [Validators.required]],
    }, { validators: passwordMatchValidator });
  }

  ngOnInit(): void {
    const usuarioString = sessionStorage.getItem('usuario');
    if (usuarioString) {
      const usuario = JSON.parse(usuarioString);
      this.completeProfileForm.patchValue({
        empresa: usuario.empresa,
        nombre: usuario.nombre,
        direccion: usuario.direccion,
        telefono: usuario.telefono,
        cargo: usuario.cargo,
      });
    }
  }

  get nombre() { return this.completeProfileForm.get("nombre"); }
  get empresa() { return this.completeProfileForm.get("empresa"); }
  get direccion() { return this.completeProfileForm.get("direccion"); }
  get telefono() { return this.completeProfileForm.get("telefono"); }
  get cargo() { return this.completeProfileForm.get("cargo"); }
  get contrasena() { return this.completeProfileForm.get("contrasena"); }
  get confirm_contrasena() { return this.completeProfileForm.get("confirm_contrasena"); }

  complete() {
    const postData = { ...this.completeProfileForm.value };
    const usuarioString = sessionStorage.getItem('usuario');
    if (usuarioString) {
      const usuario = JSON.parse(usuarioString);

      postData.email = usuario.email;
      postData.estado_invitado = 'aceptado';
      postData.rol = usuario.rol;

      delete postData.confirm_contrasena;

      this.usuarioService.updateUsuarioByEmail(postData as UsuarioRegister).subscribe({
        next: (data: any) => {
          this.router.navigate(['home']);
        },
        error: (error: any) => {
          console.log(error);
        }
      });
    }
  }
}
