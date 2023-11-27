import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ReactiveFormsModule, FormBuilder, Validators, FormGroup } from '@angular/forms';
import { Router, RouterModule } from '@angular/router';
import { passwordMatchValidator, password_pattern, name_pattern } from '../../shared/password-match.directive';
import { UsuarioService } from '../../services/usuario/usuario.service';
import { HttpClientModule } from '@angular/common/http';
import { UsuarioRegister } from '../../interfaces/usuario';
import { ToastService } from '../../services/toast/toast.service';
import { TOAST_TYPE } from '../../shared/components/toast/toast.component';

@Component({
  selector: 'app-register',
  standalone: true,
  providers: [UsuarioService],
  imports: [CommonModule, ReactiveFormsModule, RouterModule, HttpClientModule],
  templateUrl: './register.component.html',
  styleUrl: './register.component.css'
})
export class RegisterComponent implements OnInit {
  registerForm: FormGroup;

  constructor(private fb: FormBuilder, private usuarioService: UsuarioService, private router: Router, private toastService: ToastService) {
    this.registerForm = this.fb.group({
      empresa: ['', [Validators.required]],
      nombre: ['', [Validators.required, Validators.pattern(name_pattern)]],
      direccion: ['', [Validators.required]],
      telefono: ['', [Validators.required, Validators.pattern('^[\+]?[(]?[0-9]{3}[)]?[-\s\.]?[0-9]{3}[-\s\.]?[0-9]{4,6}$')]],
      email: ['', [Validators.required, Validators.email]],
      cargo: [''],
      contrasena: ['', [Validators.required, Validators.pattern(password_pattern)]],
      confirm_contrasena: ['', [Validators.required]],
    }, { validators: passwordMatchValidator });
  }

  ngOnInit(): void {
  }

  get empresa() { return this.registerForm.get("empresa"); }
  get nombre() { return this.registerForm.get("nombre"); }
  get direccion() { return this.registerForm.get("direccion"); }
  get telefono() { return this.registerForm.get("telefono"); }
  get email() { return this.registerForm.get("email"); }
  get cargo() { return this.registerForm.get("cargo"); }
  get contrasena() { return this.registerForm.get("contrasena"); }
  get confirm_contrasena() {
    return this.registerForm.get("confirm_contrasena");
  }

  submit() {
    const postData = { ...this.registerForm.value };
    delete postData.confirm_contrasena;
    postData.estado_invitado = 'autoregistrado';
    postData.rol = 'general';

    this.usuarioService.registerUsuario(postData as UsuarioRegister).subscribe({
      next: () => {
        sessionStorage.clear();
        this.router.navigate(['/login']);
        this.toastService.showToast('Registro realizado correctamente', 'Pronto recibirá un correo electrónico con las instrucciones para activar su cuenta.', TOAST_TYPE.success);
      },
      error: (error: any) => {
        if (error.status === 409) {
          this.registerForm.get('email')?.setErrors({ 'emailExists': true });
          this.toastService.showToast('Error', 'El correo electrónico ingresado ya existe.', TOAST_TYPE.error);
        } else {
          this.toastService.showToast('Error', 'Ocurrió un error al registrar el usuario.', TOAST_TYPE.error);
        }
      }
    });
  }
}
