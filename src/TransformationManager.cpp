
#include "transformations/TransformationManager.h"

void TransformationManager::add_translation(double dx, double dy) {
        Matrix translation_matrix(3, 3);
        translation_matrix.set_identity();
        translation_matrix.set_element(0, 2, dx);
        translation_matrix.set_element(1, 2, dy);

        transformation_matrix = translation_matrix * transformation_matrix;
}

void TransformationManager::apply_transformations(const std::vector<std::unique_ptr<Renderable>>& renderables) {
    this->transformed_renderables = renderables;

    for (std::unique_ptr<Renderable>& r : this->transformed_renderables) {
        r.get()->apply_transformation_matrix(this->transformation_matrix);
    }
}

void TransformationManager::reset() {
    this->transformed_renderables.clear();
    this->transformation_matrix.set_identity();
}