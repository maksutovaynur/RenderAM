#include "Renderer.hpp"
#include <iostream>
#include "SFGUI/SFGUI.hpp"
#include "SFGUI/Button.hpp"
#include "SFGUI/Window.hpp"
#include "SFGUI/Label.hpp"
#include "SFGUI/Context.hpp"
#include "SFGUI/Desktop.hpp"
#include "SFGUI/Table.hpp"
#include "SFGUI/Scale.hpp"

class Application
{
public:
    Application(int w, int h, std::string str)
    {
        sf::ContextSettings settings;
        settings.depthBits = 32;
        window = new sf::RenderWindow( sf::VideoMode(w, h), str, sf::Style::Default, settings);
        window->setVerticalSyncEnabled(true);
        window->resetGLStates();
        mouse.setPosition(sf::Vector2i(window->getSize().x/2, window->getSize().y/2), (const sf::Window&)*window);
        renderer = new am::Renderer(window, "data/shader/shader.vert", "data/shader/coolshader.pix");
    }
    ~Application()
    {
    }
    void ALotOfThings()
    {
        std::vector<std::string> sky;
        /*sky.push_back("data/textures/desert_night_rt.tga");
        sky.push_back("data/textures/desert_night_lf.tga");
        sky.push_back("data/textures/desert_night_up.tga");
        sky.push_back("data/textures/desert_night_dn.tga");
        sky.push_back("data/textures/desert_night_bk.tga");
        sky.push_back("data/textures/desert_night_ft.tga");
        sky.push_back("data/textures/mvl/mvl-rt.hdr");
        sky.push_back("data/textures/mvl/mvl-lf.hdr");
        sky.push_back("data/textures/mvl/mvl-up.hdr");
        sky.push_back("data/textures/mvl/mvl-dn.hdr");
        sky.push_back("data/textures/mvl/mvl-ft.hdr");
        sky.push_back("data/textures/mvl/mvl-bk.hdr");*/
        sky.push_back("data/textures/dirtroad/dirtroad1_rt.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_lf.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_up.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_dn.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_ft.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_bk.hdr");
        //sky.push_back("data/textures/desert_night_ft.tga");
        //sky.push_back("data/obj/heachet/Albedo.PNG");
        am::Mesh* mesh[10];
        mesh[0] = renderer->LoadMesh("simplesphere" ,"data/obj/simplesphere.obj");
        mesh[1] = renderer->LoadMesh("sphere" ,"data/obj/sphere.obj");
        mesh[2] = renderer->LoadMesh("thorus" ,"data/obj/thorus.obj");
        mesh[3] = renderer->LoadMesh("thorus" ,"data/obj/simplethorus.obj");
        mesh[4] = renderer->LoadMesh("cube" ,"data/obj/cube.obj");
        mesh[5] = renderer->LoadMesh("twoquads" ,"data/obj/twoquads.obj");
        mesh[6] = renderer->LoadMesh("monkey" ,"data/obj/monkey.obj");
        mesh[7] = renderer->LoadMesh("simplemonkey" ,"data/obj/simplemonkey.obj");
        mesh[8] = renderer->LoadMesh("hatchet" ,"data/obj/heachet/Hatchet.obj");
        mesh[9] = renderer->LoadMesh("piano" ,"data/obj/piano/piano.obj");
        am::CubeTexture* texC = renderer->LoadCubeTexture("sky", sky);
        am::Material* sk = renderer->LoadMaterial("sky", texC);
        renderer->CreateSky(sk);
        //am::Coords3f coor;
        //coor.LocalScale(am::Vector3f(renderer->camera.farP, renderer->camera.farP, renderer->camera.farP));
        //renderer->LoadEntity(mesh[0], coor, sk);
        //mesh->TransformVertices(Matrix3x3f(0.8f))
        //mesh[8]->ClearIdentityVertex();
        //mesh[8]->AlignVertices();
        //mesh[8]->SaveToFile("data/obj/truba2.obj", VERTEX_ONLY);
        am::Texture* texture;
        am::Entity* entity;
        am::Coords3f coords;
        texture = renderer->LoadTexture("dirtroad" ,"data/textures/dirtroad.hdr");
        am::Texture* albedo = renderer->LoadTexture("hatchAl" ,"data/obj/heachet/Albedo.PNG");
        am::Texture* refl = renderer->LoadTexture("hatchMt", "data/obj/heachet/Metalness.PNG");
        am::Texture* micr = renderer->LoadTexture("hatchGl", "data/obj/heachet/Gloss.PNG");
        am::Texture* fres = renderer->LoadTexture("hatchFr", "data/obj/heachet/Metalness.PNG");
        am::Texture* norm = renderer->LoadTexture("hatchNr", "data/obj/heachet/Normal_Detailed.PNG");
        am::Material* mat[3];
        mat[0] = renderer->LoadMaterial("hatchet", 2);//albedo, micr, refl, fres, norm
        renderer->AddTextureToMaterial("hatchet", "hatchAl", "albedo");
        renderer->AddTextureToMaterial("hatchet", "hatchMt", "reflectivity");
        renderer->AddTextureToMaterial("hatchet", "hatchGl", "microsurface");
        renderer->AddTextureToMaterial("hatchet", "hatchFr", "fresnel");
        renderer->AddTextureToMaterial("hatchet", "hatchNr", "normalMap");
        mat[1] = renderer->LoadMaterial("something", texture);
        albedo = renderer->LoadTexture("white", "data/textures/yellow.png");
        refl = renderer->LoadTexture("yellow", "data/textures/yellow.png");
        fres = renderer->LoadTexture("green", "data/textures/green.png");
        micr = renderer->LoadTexture("m", "data/textures/m.png");
        mat[2] = renderer->LoadMaterial("new", albedo, micr, refl, fres);
        {
        }
        int a = 0;
        int b = 15;
        int c = 15;
        if(true)
        {
            coords.LocalScale(am::Vector3f(5.f, 5.f, 5.f));
            entity = renderer->LoadEntity("hatchet", mesh[8], coords, mat[0], texC);
        }
        else
        {
            //coords.LocalScale(am::Vector3f(0.08f, 0.08f, 0.08f));
            for(int k = 0; k<a; k++)
                for(int i = -b+1; i<b; i++)
            {
                for(int j = -c+1; j<c; j++)
                {
                    coords.pos.x = i*4;
                    coords.pos.y = j*4;
                    coords.pos.z = -k*4;
                    entity = renderer->LoadEntity("", mesh[random()%9], coords, mat[0]);
                }
            }
        }
    }
    void Start()
    {
        ALotOfThings();
        std::vector<float> s;
        s.push_back(1.f);
        s.push_back(0.03f);
        float alpha = 0;
        sf::Clock clock;
        renderer->camera.coords.pos = am::Vector3f(0, 0.f, 5.3f);
        renderer->camera.coords.SetOrientation();

        while(window->isOpen())
        {
            sf::Event event;
            while(window->pollEvent(event))
            {
                if(event.type==sf::Event::Closed)
                {
                    if(renderer!=NULL) delete(renderer);
                    renderer = NULL;
                    window->close();
                    break;
                }
                if(event.type==sf::Event::KeyPressed)
                {
                    if(event.key.code==sf::Keyboard::Escape)
                    {
                        if(renderer!=NULL) delete(renderer);
                        renderer = NULL;
                        window->close();
                        break;
                    }
                }
            }
            {
                float dt = clock.getElapsedTime().asSeconds();
                clock.restart();
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) s[0]*=1.1f;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) s[0]/=1.1f;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) s[1]*=1.1f;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) s[1]/=1.1f;
                    renderer->SetExp(s[0]);
                    renderer->SetSkyExp(s[1]);
                }
                char str[256];
                am::Vector3f pos = renderer->camera.coords.pos;
                sprintf(str, "PBR( %8f, %8f, %8f )\t\t\t\tPARAMS{ %4f }  \t\t\t\t\t\tFPS = %3f",
                        pos.x, pos.y, pos.z, s[0], 1/dt);
                window->setTitle(str);
                //guiWindow->Update(dt);
                {
                    am::Vector3f vec = am::Vector3f(0, 0, 0);
                    float rotX = 0, rotY = 0, sY = 0;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) vec.z = -1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) vec.z = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) vec.x = -1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) vec.x = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) rotX = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) rotX = -1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotY = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotY = -1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) sY = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) sY = -1;
                    float v;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) v = 1.f;
                    else v = 10.f;
                    renderer->camera.coords.LocalTranslate(vec, v*dt);
                    renderer->camera.coords.GlobalTranslate(am::Vector3f(0, sY, 0), v*dt);
                    renderer->camera.coords.GlobalRotate(am::Vector3f(0, -1 ,0), dt*rotY);
                    renderer->camera.coords.LocalRotate(am::Vector3f(-1, 0, 0), dt*rotX);
                    renderer->camera.coords.CutLessThan(0.0000001f);
                }
                {
                    am::Vector3f vec = am::Vector3f(0, 0, 0);
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) vec.x = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) vec.x = -1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) vec.y = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) vec.y = -1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) vec.z = 1;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) vec.z = -1;
                    int n = renderer->GetEntityCount();
                    for(int i = 0; i < n; i++)
                    {
                        renderer->GetEntity(i)->coords.LocalRotate(vec, dt);
                    }
                }
            }
            window->clear(sf::Color::White);
            window->pushGLStates();
            renderer->RenderFrame();
            window->popGLStates();
            window->display();
        }
    }
    class MyTable
    {
    public:
        MyTable(am::Renderer* rend, std::string name)
        {
            table = sfg::Table::Create();
            delta = 4;
            size = 0;
            this->rend = rend;
            this->name = name;
        }
        void AddParameter(std::string name, float r = 0, float g = 0, float b = 0)
        {
            params.push_back(new Parameter(name, r, g, b));
            table->Attach(sfg::Label::Create(name), sf::Rect<sf::Uint32>(0, size, 5, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(sfg::Label::Create("r"), sf::Rect<sf::Uint32>(0, size+1, 1, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(sfg::Label::Create("g"), sf::Rect<sf::Uint32>(0, size+2, 1, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(sfg::Label::Create("b"), sf::Rect<sf::Uint32>(0, size+3, 1, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(params[params.size()-1]->r, sf::Rect<sf::Uint32>(1, size+1, 3, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(params[params.size()-1]->g, sf::Rect<sf::Uint32>(1, size+2, 3, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(params[params.size()-1]->b, sf::Rect<sf::Uint32>(1, size+3, 3, 1),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            table->Attach(params[params.size()-1]->but, sf::Rect<sf::Uint32>(4, size+1, 2, 3),
                          sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
            size+=delta;
            table->SetColumnSpacing(1, 150);
            table->SetColumnSpacing(4, 30);
            rend->LoadTexture(this->name+"-"+params[params.size()-1]->name, params[params.size()-1]->GetColor());
        }

        sfg::Table::Ptr GetTable()
        {
            return table;
        }
        void Update()
        {
            /*for(int i = 1; i< params.size()-1; i++)
            {
                params[i]->g->SetValue(params[i]->r->GetValue());
                params[i]->b->SetValue(params[i]->r->GetValue());
            }*/
            for(int i = 0; i < params.size(); i++)
            {
                sf::Color color = params[i]->SetParameter();
                rend->UpdateTexture(this->name+"-"+params[i]->name, color);
            }
        }
        void Begin(am::CubeTexture* sky)
        {
            am::Material* material = rend->LoadMaterial(name, 2);
            for(int i = 0; i < params.size(); i++)
            {
                rend->AddTextureToMaterial(name, name+"-"+params[i]->name, params[i]->name);
            }
            am::Mesh* mesh = rend->LoadMesh("", "data/obj/simplesphere.obj");
            am::Coords3f coords;
            coords.LocalScale(am::Vector3f(0.3f, 0.3f, 0.3f));
            coords.LocalScale(am::Vector3f(10.f, 10.f, 10.f));
            am::Entity* ent = rend->LoadEntity(name, mesh, coords, material, sky);
        }

    private:
        class Parameter
        {
        public:
            Parameter(std::string name, float r = 0, float g = 0, float b = 0)
            {
                this->r = sfg::Scale::Create(0.f, 1.f, 0.001f, sfg::Scale::Orientation::HORIZONTAL);
                this->g = sfg::Scale::Create(0.f, 1.f, 0.001f, sfg::Scale::Orientation::HORIZONTAL);
                this->b = sfg::Scale::Create(0.f, 1.f, 0.001f, sfg::Scale::Orientation::HORIZONTAL);
                //this->r->GetSignal(sfg::Scale::OnMouseMove).Connect(std::bind(&Application::MyTable::Parameter::SetParameter, this));
                //this->g->GetSignal(sfg::Scale::OnMouseMove).Connect(std::bind(&Application::MyTable::Parameter::SetParameter, this));
                //this->b->GetSignal(sfg::Scale::OnMouseMove).Connect(std::bind(&Application::MyTable::Parameter::SetParameter, this));
                this->r->SetValue(r);
                this->g->SetValue(g);
                this->b->SetValue(b);
                this->r->SetId(name);
                this->g->SetId(name);
                this->b->SetId(name);
                this->name = name;
                this->but = sfg::Button::Create("");
                but->SetId(name);
                std::string str("BackgroundColor");
                sfg::Context::Get().GetEngine().SetProperty("Button#"+name , str, sf::Color::Red);
            }
            sf::Color SetParameter()
            {
                std::string str("BackgroundColor");
                sf::Color color(this->r->GetValue()*255, this->g->GetValue()*255, this->b->GetValue()*255);
                sfg::Context::Get().GetEngine().SetProperty("Button#"+this->name , str, color);
                return color;
            }
            sf::Color GetColor()
            {
                float cr = r->GetValue();
                float cg = g->GetValue();
                float cb = b->GetValue();
                return sf::Color(255*cr, 255*cg, 255*cb);
            }
            std::string name;
            sfg::Scale::Ptr r, g, b;
            sfg::Button::Ptr but;
        };
        std::vector<Parameter*>params;
        sfg::Table::Ptr table;
        int delta, size;
        am::Renderer* rend;
        std::string name;
    };

    void RunTest()
    {
        std::vector<float> s;
        s.push_back(1.f);
        s.push_back(0.03f);
        sf::Clock clock;
        std::vector<std::string> sky;
        sky.push_back("data/textures/dirtroad/dirtroad1_rt.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_lf.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_up.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_dn.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_ft.hdr");
        sky.push_back("data/textures/dirtroad/dirtroad1_bk.hdr");
        am::CubeTexture* texC = renderer->LoadCubeTexture("sky", sky);
        am::Material    * sk = renderer->LoadMaterial("sky", texC);
        renderer->CreateSky(sk);
        //ALotOfThings();
        window->pushGLStates();
        sfg::SFGUI sfgui;
        MyTable table(this->renderer, "sp1");
        window->popGLStates();
        table.AddParameter("albedo", 0.5f, 0.5f, 0.5f);
        table.AddParameter("reflectivity", 0.5f, 0.5f, 0.5f);
        table.AddParameter("microsurface", 0.5f, 0.5f, 0.5f);
        table.AddParameter("fresnel", 0.5f, 0.5f, 0.5f);
        table.AddParameter("normalMap", 0.5f, 0.5f, 1.f);
        table.Begin(texC);
        window->pushGLStates();
        auto guiWindow = sfg::Window::Create();
        guiWindow->SetTitle("Change material options");
        guiWindow->SetPosition(sf::Vector2f(0.f, 100.f));
        guiWindow->Add(table.GetTable());
        sfg::Scale::Ptr angle = sfg::Scale::Create(0.f, M_PI, 0.00001f, sfg::Scale::Orientation::HORIZONTAL);
        sfg::Table::Ptr tbl = sfg::Table::Create();
        tbl->Attach(sfg::Label::Create("Angle"), sf::Rect<sf::Uint32>(0, 0, 1, 1),
                              sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
        tbl->Attach(angle, sf::Rect<sf::Uint32>(1, 0, 3, 1),
                              sfg::Table::FILL|sfg::Table::EXPAND, sfg::Table::FILL|sfg::Table::EXPAND);
        tbl->SetColumnSpacing(2, 550);
        auto guiWindow2 = sfg::Window::Create();
        guiWindow2->SetTitle( "Change angle of View");
        guiWindow2->Add(tbl);
        sfg::Desktop desktop;
        desktop.Add(guiWindow);
        desktop.Add(guiWindow2);
        window->popGLStates();
        window->setVerticalSyncEnabled(true);
        renderer->camera.coords.pos = am::Vector3f(0.f, 0.f, 5.3f);
        renderer->camera.coords.SetOrientation();
        float currentTime = 0;
        while(window->isOpen())
        {
            sf::Event event;
            while(window->pollEvent(event))
            {
                if(event.type==sf::Event::Closed)
                {
                    delete(renderer);
                    window->close();
                }
                else if(event.type==sf::Event::KeyPressed)
                {
                    if(event.key.code==sf::Keyboard::Escape)
                    {
                        delete(renderer);
                        window->close();
                    }
                }
                else desktop.HandleEvent(event);
            }
            float dt = clock.getElapsedTime().asSeconds();
            clock.restart();
            currentTime+=dt;
            renderer->SetCurrentTime(currentTime);
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) s[0]*=1.1f;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) s[0]/=1.1f;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) s[1]*=1.1f;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) s[1]/=1.1f;
                renderer->SetExp(s[0]);
                renderer->SetSkyExp(s[1]);
            }
            char str[256];
            am::Vector3f pos = renderer->camera.coords.pos;
            sprintf(str, "PBR( %8f, %8f, %8f )\t\t\t\tPARAMS{%4f, %4f }  \t\t\t\t\t\tFPS = %3f",
                    pos.x, pos.y, pos.z, s[0], s[1],  1/dt);
            window->setTitle(str);
            //guiWindow->Update(dt);
            {
                am::Vector3f vec = am::Vector3f(0, 0, 0);
                float rotX = 0, rotY = 0, sY = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) vec.z = -1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) vec.z = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) vec.x = -1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) vec.x = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) rotX = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) rotX = -1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotY = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotY = -1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) sY = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) sY = -1;
                float v;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) v = 1.f;
                else v = 10.f;
                renderer->camera.coords.LocalTranslate(vec, v*dt);
                renderer->camera.coords.GlobalTranslate(am::Vector3f(0, sY, 0), v*dt);
                renderer->camera.coords.GlobalRotate(am::Vector3f(0, -1 ,0), dt*rotY);
                renderer->camera.coords.LocalRotate(am::Vector3f(-1, 0, 0), dt*rotX);
                renderer->camera.coords.CutLessThan(0.0000001f);
            }
            {
                am::Entity* ent = renderer->GetEntity("sky");
                float a = angle->GetValue();
                ent->coords.SetOrientation(am::Vector3f(cos(a), 0.f, -sin(a)));
                ent->coords.LocalScale(am::Vector3f(renderer->camera.farP, renderer->camera.farP, renderer->camera.farP));
                am::Matrix3x3f m = ent->coords.GetMatrix();
                renderer->envModelMatrix = m;
                ent = renderer->GetEntity("sp1");
            }
            renderer->camera.coords.CutLessThan(0.0000001f);
            renderer->SetView();
            renderer->SetExp(s[0]);
            renderer->SetSkyExp(s[1]);
            window->clear(sf::Color::White);
            window->pushGLStates();
            renderer->RenderFrame();
            window->popGLStates();
            table.Update();
            desktop.Update(1.f);
            sfgui.Display(*window);
            window->display();
        }

    }

private:
    sf::RenderWindow* window;
    sf::Mouse mouse;
    am::Renderer* renderer;
};
