pipeline {
    agent any

    stages {
        stage('Build Docker Image') {
            steps {
                script {
                    dimg = docker.build('cclimg:latest', 'docker')
                }
            }
        }

        stage('Run Build and Test In Container') {
            steps {
                script {
                    dimg.inside('--privileged --cap-add=all') {
                        sh '''
                            mkdir -p build
                            cd build
                            cmake ..
                            make -j(nproc)
                            ctest --output-on-failure
                        '''
                    }
                }
            }
        }
    }
}