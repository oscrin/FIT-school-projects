<?php

use Illuminate\Database\Seeder;

use App\User;

class UsersSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        User::create([
            'name' => 'Miroslav Kažimír',
            'about' => 'Student FIT VUT',
            'email' => 'mirec.kazimir@gmail.com',
            'password' => bcrypt('projektik'),
        ]);

        User::create([
            'name' => 'Jan Kubica',
            'about' => 'Student FIT VUT',
            'email' => 'jan.kubica@itu.fit',
            'password' => bcrypt('projektik'),
        ]);

        User::create([
            'name' => 'Viktor Kovařík',
            'about' => 'Student FIT VUT',
            'email' => 'viktor.kovarik@itu.fit',
            'password' => bcrypt('projektik'),
        ]);

    }
}
