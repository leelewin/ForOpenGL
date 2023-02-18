//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Intersection p = intersect(ray);
    // std::cout << p.happened << std::endl;
    if (p.happened)
        return shade(p, normalize(ray.origin - p.coords)); //wo为什么取这样的数？？
    else
        return Vector3f(0.0f);

}

Vector3f Scene::shade(const Intersection& p, const Vector3f& wo) const 
{
    if (p.m->hasEmission())
        return p.m->getEmission();
    Vector3f L_dir(0.0f);
    Intersection inter;
    float pdf_light;
    sampleLight(inter, pdf_light);
    Vector3f x = inter.coords;
    Vector3f emit = inter.emit;
    Vector3f ws = normalize(x - p.coords);
    Vector3f N_s = inter.normal;
    float d = (x - p.coords).norm();

    Intersection mid = intersect(Ray(p.coords, ws));
    if ((mid.coords - p.coords).norm() - d > -0.001) {   //为什么像L这种的变量的是Vector3f类型的？
        L_dir = emit * p.m->eval(wo, ws, p.normal) * dotProduct(ws, p.normal) * \
                               dotProduct(-ws, N_s) / (d * d) / pdf_light;
    }

    Vector3f L_indir(0.0f);
    if (get_random_float() > RussianRoulette)
        return L_dir + L_indir;

    Vector3f wi = p.m->sample(wo, p.normal).normalized();
    Intersection q = intersect(Ray(p.coords, wi)); 
    if (q.happened && !q.m->hasEmission()) {
        L_indir = shade(q, wi) * p.m->eval(wo, wi, p.normal) * dotProduct(wi, p.normal) /
                             p.m->pdf(wo, wi, p.normal) / RussianRoulette;
    }

    return L_dir + L_indir;

}

