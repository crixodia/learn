import { Routes } from '@angular/router';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { HomeComponent } from './components/home/home.component';
import { RecoverComponent } from './components/recover/recover.component';
import { AdminComponent } from './components/admin/admin.component';
import { authGuard, loggedGuard } from './guards/auth.guard';
import { UpdateProfileComponent } from './components/update-profile/update-profile.component';
import { SignupComponent } from './shared/components/signup/signup.component';
import { UsersComponent } from './components/admin/users/users.component';
import { ResourcesComponent } from './components/admin/resources/resources.component';
import { SpacesComponent } from './components/admin/spaces/spaces.component';
import { RegionsComponent } from './components/admin/regions/regions.component';
import { ParkingsComponent } from './components/admin/parkings/parkings.component';
import { BuildingsComponent } from './components/admin/buildings/buildings.component';
import { FloorsComponent } from './components/admin/floors/floors.component';
import { BookingsComponent } from './components/admin/bookings/bookings.component';
import { RegisteredComponent } from './components/admin/registered/registered.component';

export const routes: Routes = [
    {
        path: '',
        component: SignupComponent,
        children: [{
            path: 'register',
            component: RegisterComponent,
            canActivate: [loggedGuard]
        }, {
            path: 'login',
            component: LoginComponent,
            canActivate: [loggedGuard]
        }, {
            path: 'recover',
            component: RecoverComponent
        }, {
            path: 'update-profile',
            component: UpdateProfileComponent,
            canActivate: [authGuard]
        }
        ]
    },
    {
        path: 'home',
        component: HomeComponent,
        canActivate: [authGuard]
    },
    {
        path: 'admin',
        component: AdminComponent,
        canActivate: [authGuard],
        children: [{
            path: 'users',
            component: UsersComponent,
        }, {
            path: 'spaces',
            component: SpacesComponent
        }, {
            path: 'resources',
            component: ResourcesComponent
        }, {
            path: 'registered',
            component: RegisteredComponent
        }, {
            path: 'regions',
            component: RegionsComponent
        }, {
            path: 'parkings',
            component: ParkingsComponent
        }, {
            path: 'buildings',
            component: BuildingsComponent
        }, {
            path: 'floors',
            component: FloorsComponent
        }, {
            path: 'bookings',
            component: BookingsComponent
        }]
    },
    {
        path: '**',
        redirectTo: 'not-found'
    }
];
